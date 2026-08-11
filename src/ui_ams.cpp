#include "ui_ams.h"

void renderAmsScreen(DisplayDriver &display, const PrinterState &state) {
  display.clear();
  display.drawText(4, 2, "AMS", 0xFFFF, 1);
  for (int i = 0; i < 4; i++) {
    const AmsTrayState &tray = state.trays[i];
    const String marker = tray.active ? "* " : "  ";
    const String label = tray.present ? (marker + String(i + 1) + " " + tray.type + " " + tray.color) : ("  " + String(i + 1) + " --");
    display.drawText(12, 28 + i * 24, label, 0xFFFF, 1);
  }
}
