#pragma once

#include <Arduino.h>
#include "printer_state.h"

class WifiManager {
 public:
  void begin();
  void loop(PrinterState &state);

 private:
  uint32_t lastAttemptMs = 0;
  bool wasConnected = false;
  void connect();
};
