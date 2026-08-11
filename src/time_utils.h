#pragma once

#include <Arduino.h>

String formatMinutes(int minutes);
String formatSeconds(uint32_t seconds);
int clampPercent(int value);
