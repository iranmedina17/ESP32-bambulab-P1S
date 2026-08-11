#include "ui.h"
#include "config.h"
#include "ui_dashboard.h"

void Ui::begin(DisplayDriver *driver) {
  display = driver;
  if (display) {
    display->clear();
    display->drawText(82, 48, DEVICE_NAME, 0xFFFF, 2);
    display->drawText(68, 80, "P1S DESKTOP MONITOR", 0xFFFF, 1);
  }
}

void Ui::loop(const PrinterState &state) {
  if (!display || millis() - lastRefreshMs < UI_REFRESH_MS) return;
  lastRefreshMs = millis();

  const String frameKey = state.rawState + "|" + state.projectName + "|" + String(state.progress) + "|" +
                          String(state.remainingMinutes) + "|" + String(state.currentLayer) + "|" +
                          String(state.wifiConnected) + String(state.mqttConnected) + String(state.printerConnected);
  if (frameKey == lastFrameKey) return;
  lastFrameKey = frameKey;
  renderDashboardScreen(*display, state);
}
