#pragma once

#include <Arduino.h>

enum PrinterVisualState : uint8_t {
  PRINTER_IDLE,
  PRINTER_PREPARING,
  PRINTER_PRINTING,
  PRINTER_PAUSED,
  PRINTER_FINISHED,
  PRINTER_ERROR,
  PRINTER_OFFLINE
};

struct AmsTrayState {
  bool present = false;
  bool active = false;
  String type;
  String color;
  String name;
};

struct PrinterState {
  String projectName = "";
  String filename = "";
  String rawState = "UNKNOWN";
  String stage = "";

  int progress = 0;
  int currentLayer = 0;
  int totalLayers = 0;

  int remainingMinutes = -1;
  int elapsedMinutes = -1;
  int estimatedTotalMinutes = -1;

  float nozzleTemp = NAN;
  float nozzleTarget = NAN;
  float bedTemp = NAN;
  float bedTarget = NAN;
  float chamberTemp = NAN;

  String speedMode = "";
  String filamentType = "";
  String filamentColor = "";

  int activeAms = -1;
  int activeTray = -1;
  bool amsConnected = false;
  AmsTrayState trays[4];

  bool wifiConnected = false;
  bool mqttConnected = false;
  bool printerConnected = false;

  String errorCode = "";
  String errorMessage = "";
  String hmsCode = "";

  uint32_t printStartedAtMs = 0;
  uint32_t lastPrinterMessageMs = 0;
  uint32_t finishedAtMs = 0;
};

PrinterVisualState visualStateFor(const PrinterState &state);
String visualStateText(PrinterVisualState state);
void updateDerivedTimes(PrinterState &state, uint32_t nowMs);
void updatePrinterTimeout(PrinterState &state, uint32_t nowMs);
