#pragma once

#include "display_driver.h"
#include "printer_state.h"

class Ui {
 public:
  void begin(DisplayDriver *driver);
  void loop(const PrinterState &state);

 private:
  DisplayDriver *display = nullptr;
  uint32_t lastRefreshMs = 0;
  String lastFrameKey = "";
};
