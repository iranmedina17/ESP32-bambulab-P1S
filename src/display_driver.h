#pragma once

#include <Arduino.h>

class DisplayDriver {
 public:
  void begin();
  void clear();
  void drawText(int x, int y, const String &text, uint16_t color = 0xFFFF, uint8_t size = 1);
  void drawProgress(int x, int y, int w, int h, int percent, uint16_t color);
};
