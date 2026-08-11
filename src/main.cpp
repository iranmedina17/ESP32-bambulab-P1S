#include <Arduino.h>
#include "bambu_mqtt.h"
#include "config.h"
#include "demo_data.h"
#include "display_driver.h"
#include "logger.h"
#include "printer_state.h"
#include "ui.h"
#include "wifi_manager.h"

static PrinterState printerState;
static WifiManager wifiManager;
static BambuMqtt bambuMqtt;
static DisplayDriver displayDriver;
static Ui ui;

void setup() {
  Serial.begin(115200);
  delay(100);
  logInfo("BOOT", "Starting DODO PRINT P1S Desktop Monitor");
  displayDriver.begin();
  ui.begin(&displayDriver);

#if DEMO_MODE
  logInfo("DEMO", "Demo mode enabled. MQTT connection disabled.");
#else
  wifiManager.begin();
  bambuMqtt.begin(&printerState);
#endif
}

void loop() {
  const uint32_t now = millis();

#if DEMO_MODE
  updateDemoState(printerState);
#else
  wifiManager.loop(printerState);
  bambuMqtt.loop();
  updatePrinterTimeout(printerState, now);
#endif

  updateDerivedTimes(printerState, now);
  ui.loop(printerState);
  yield();
}
