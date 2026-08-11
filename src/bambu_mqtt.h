#pragma once

#include <Arduino.h>
#include "printer_state.h"

class BambuMqtt {
 public:
  void begin(PrinterState *state);
  void loop();

 private:
  PrinterState *printerState = nullptr;
  uint32_t lastConnectAttemptMs = 0;
  bool wasConnected = false;
  void connect();
  void onMessage(char *topic, uint8_t *payload, unsigned int length);
};
