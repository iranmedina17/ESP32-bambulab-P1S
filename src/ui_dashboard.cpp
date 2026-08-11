#include "ui_dashboard.h"
#include "config.h"
#include "time_utils.h"

static String tempText(float current, float target) {
  if (isnan(current)) return "--";
  String out = String(static_cast<int>(round(current))) + "C";
  if (!isnan(target) && target > 0) out += "/" + String(static_cast<int>(round(target))) + "C";
  return out;
}

static String tempCompact(float current, float target) {
  if (isnan(current)) return "--";
  String out = String(static_cast<int>(round(current)));
  if (!isnan(target) && target > 0) out += "/" + String(static_cast<int>(round(target)));
  return out;
}

void renderDashboardScreen(DisplayDriver &display, const PrinterState &state) {
  display.clear();
  const String title = state.projectName.length() ? state.projectName : "Bambu Lab P1S";
  const String filament = state.filamentType.length() ? state.filamentType + " " + state.filamentColor : "FIL --";
  const String amsText = state.amsConnected ? ("AMS " + String(state.activeTray + 1)) : "AMS --";
  const String layerText = state.currentLayer > 0 ? ("CAPA " + String(state.currentLayer) + "/" + String(state.totalLayers)) : "CAPA --";

  display.drawText(4, 2, "DODO PRINT - Bambu P1S TARS", 0xFFFF, 1);
  display.drawText(6, 18, title.substring(0, 25), 0xFFFF, 2);
  display.drawProgress(6, 43, 220, 20, clampPercent(state.progress), 0x07E0);
  display.drawText(236, 38, String(clampPercent(state.progress)) + "%", 0xFFFF, 3);

  display.drawText(8, 72, "RESTANTE", 0xFFFF, 1);
  display.drawText(8, 86, formatMinutes(state.remainingMinutes), 0xFFFF, 3);

  display.drawText(150, 76, "NOZ " + tempCompact(state.nozzleTemp, state.nozzleTarget), 0xFFFF, 2);
  display.drawText(150, 102, "BED " + tempCompact(state.bedTemp, state.bedTarget), 0xFFFF, 2);

  display.drawText(8, 132, layerText.substring(0, 16), 0xFFFF, 1);
  display.drawText(118, 132, (amsText + " " + filament).substring(0, 20), 0xFFFF, 1);

  display.drawText(10, 156, String("WiFi ") + (state.wifiConnected ? "OK" : "--"), 0xFFFF, 1);
  display.drawText(112, 156, String("MQTT ") + (state.mqttConnected ? "OK" : "--"), 0xFFFF, 1);
  display.drawText(222, 156, String("P1S ") + (state.printerConnected ? "OK" : "--"), 0xFFFF, 1);
}
