#include "display_driver.h"
#include "config.h"
#include "logger.h"

#if USE_REAL_DISPLAY
#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>
#include <SPI.h>

static Adafruit_ST7789 tft(TFT_CS_PIN, TFT_DC_PIN, TFT_RST_PIN);

static uint16_t mapColor(uint16_t color) {
  switch (color) {
    case 0x0000: return ST77XX_BLACK;
    case 0xFFFF: return ST77XX_WHITE;
    case 0x07E0: return ST77XX_GREEN;
    case 0xF800: return ST77XX_RED;
    case 0xFFE0: return ST77XX_YELLOW;
    default: return color;
  }
}
#endif

void DisplayDriver::begin() {
#if USE_REAL_DISPLAY
  SPI.begin(TFT_SCLK_PIN, -1, TFT_MOSI_PIN, TFT_CS_PIN);
  if (TFT_BL_PIN >= 0) {
    pinMode(TFT_BL_PIN, OUTPUT);
    digitalWrite(TFT_BL_PIN, HIGH);
  }
  tft.init(DISPLAY_NATIVE_WIDTH, DISPLAY_NATIVE_HEIGHT);
  tft.setRotation(DISPLAY_ROTATION);
  tft.invertDisplay(DISPLAY_INVERT_COLORS);
  tft.setTextWrap(false);
  tft.fillScreen(ST77XX_BLACK);
  logInfo("DISPLAY", String(DISPLAY_CONTROLLER) + " real display enabled on " + DISPLAY_WIDTH + "x" + DISPLAY_HEIGHT);
#else
  logInfo("DISPLAY", String(DISPLAY_CONTROLLER) + " " + DISPLAY_WIDTH + "x" + DISPLAY_HEIGHT + " mock display enabled");
#endif
}

void DisplayDriver::clear() {
#if USE_REAL_DISPLAY
  tft.fillScreen(ST77XX_BLACK);
#endif
}

void DisplayDriver::drawText(int x, int y, const String &text, uint16_t color, uint8_t size) {
  (void)x;
  (void)y;
  (void)color;
  (void)size;
#if USE_REAL_DISPLAY
  tft.setTextColor(mapColor(color), ST77XX_BLACK);
  tft.setTextSize(size);
  tft.setCursor(x, y);
  tft.print(text);
#else
  Serial.println(text);
#endif
}

void DisplayDriver::drawProgress(int x, int y, int w, int h, int percent, uint16_t color) {
  (void)x;
  (void)y;
  (void)w;
  (void)h;
  (void)color;
#if USE_REAL_DISPLAY
  tft.drawRect(x, y, w, h, ST77XX_WHITE);
  const int fill = (w - 2) * percent / 100;
  tft.fillRect(x + 1, y + 1, fill, h - 2, mapColor(color));
  if (fill < w - 2) {
    tft.fillRect(x + 1 + fill, y + 1, (w - 2) - fill, h - 2, ST77XX_BLACK);
  }
#else
  Serial.print("Progress: ");
  Serial.print(percent);
  Serial.println('%');
#endif
}
