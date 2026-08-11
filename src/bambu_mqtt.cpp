#include "bambu_mqtt.h"
#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include "bambu_parser.h"
#include "config.h"
#include "logger.h"

static WiFiClientSecure secureClient;
static PubSubClient mqttClient(secureClient);
static BambuMqtt *activeInstance = nullptr;

void BambuMqtt::begin(PrinterState *state) {
  printerState = state;
  activeInstance = this;
#if MQTT_TLS_INSECURE
  secureClient.setInsecure();
#endif
  mqttClient.setServer(PRINTER_IP, MQTT_PORT);
  mqttClient.setBufferSize(8192);
  mqttClient.setCallback([](char *topic, uint8_t *payload, unsigned int length) {
    if (activeInstance) activeInstance->onMessage(topic, payload, length);
  });
}

void BambuMqtt::connect() {
  if (!printerState || !printerState->wifiConnected) return;
  lastConnectAttemptMs = millis();
  const String clientId = String(DEVICE_NAME) + "-esp32";
  logInfo("MQTT", "Connecting to P1S local broker");

  if (!mqttClient.connect(clientId.c_str(), MQTT_USERNAME, PRINTER_ACCESS_CODE)) {
    logWarn("MQTT", "Connect failed, rc=" + String(mqttClient.state()));
    return;
  }

  const String serialOrWildcard = String(PRINTER_SERIAL).length() ? String(PRINTER_SERIAL) : "+";
  const String reportTopic = String("device/") + serialOrWildcard + "/report";
  if (mqttClient.subscribe(reportTopic.c_str())) {
    logInfo("MQTT", "Subscribed to " + reportTopic);
  } else {
    logWarn("MQTT", "Subscribe failed");
  }
}

void BambuMqtt::loop() {
  if (!printerState) return;
  if (!printerState->wifiConnected) {
    printerState->mqttConnected = false;
    mqttClient.disconnect();
    return;
  }

  if (!mqttClient.connected()) {
    printerState->mqttConnected = false;
    if (wasConnected) logWarn("MQTT", "Connection lost");
    if (millis() - lastConnectAttemptMs >= MQTT_RETRY_MS) connect();
  } else {
    printerState->mqttConnected = true;
    if (!wasConnected) logInfo("MQTT", "Connected");
    mqttClient.loop();
  }
  wasConnected = mqttClient.connected();
}

void BambuMqtt::onMessage(char *topic, uint8_t *payload, unsigned int length) {
  (void)topic;
  if (!printerState) return;
  static bool firstReportLogged = false;
  if (parsePrinterReport(payload, length, *printerState) && !firstReportLogged) {
    firstReportLogged = true;
    logInfo("MQTT", "Report received from P1S");
    logInfo("PRINT", "Project: " + printerState->projectName);
    logInfo("PRINT", "Progress: " + String(printerState->progress) + "%");
  }
}
