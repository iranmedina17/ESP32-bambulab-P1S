#include "time_utils.h"

String formatMinutes(int minutes) {
  if (minutes < 0) return "--:--";
  const int hours = minutes / 60;
  const int mins = minutes % 60;
  char buffer[8];
  snprintf(buffer, sizeof(buffer), "%02d:%02d", hours, mins);
  return String(buffer);
}

String formatSeconds(uint32_t seconds) {
  return formatMinutes(static_cast<int>(seconds / 60));
}

int clampPercent(int value) {
  if (value < 0) return 0;
  if (value > 100) return 100;
  return value;
}
