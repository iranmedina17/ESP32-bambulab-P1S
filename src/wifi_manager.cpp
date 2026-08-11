#include "wifi_manager.h"
#include <WiFi.h>
#include "config.h"
#include "logger.h"

void WifiManager::begin() {
  WiFi.mode(WIFI_STA);
  WiFi.setAutoReconnect(true);
  connect();
}

void WifiManager::connect() {
  lastAttemptMs = millis();
  logInfo("WIFI", "Connecting to configured SSID");
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
}

void WifiManager::loop(PrinterState &state) {
  const bool connected = WiFi.status() == WL_CONNECTED;
  state.wifiConnected = connected;

  if (connected && !wasConnected) {
    logInfo("WIFI", "Connected, IP " + WiFi.localIP().toString());
  } else if (!connected && wasConnected) {
    logWarn("WIFI", "Connection lost");
  }

  if (!connected && millis() - lastAttemptMs >= WIFI_RETRY_MS) {
    connect();
  }

  wasConnected = connected;
}
