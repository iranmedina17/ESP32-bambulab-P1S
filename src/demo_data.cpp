#include "demo_data.h"

void updateDemoState(PrinterState &state) {
  const uint32_t now = millis();
  state.wifiConnected = true;
  state.mqttConnected = true;
  state.printerConnected = true;
  state.lastPrinterMessageMs = now;
  state.projectName = "Chimuelo - Llaveros";
  state.filename = "chimuelo_llaveros.3mf";
  state.rawState = "RUNNING";
  state.progress = 68 + ((now / 10000UL) % 5);
  state.currentLayer = 184 + ((now / 15000UL) % 3);
  state.totalLayers = 270;
  state.remainingMinutes = 47;
  state.elapsedMinutes = 102 + (now / 60000UL);
  state.nozzleTemp = 220;
  state.nozzleTarget = 220;
  state.bedTemp = 55;
  state.bedTarget = 55;
  state.chamberTemp = 36;
  state.speedMode = "Standard";
  state.amsConnected = true;
  state.activeAms = 0;
  state.activeTray = 1;
  state.filamentType = "PLA";
  state.filamentColor = "NEGRO";
  state.trays[1].present = true;
  state.trays[1].active = true;
  state.trays[1].type = "PLA";
  state.trays[1].color = "NEGRO";
}
