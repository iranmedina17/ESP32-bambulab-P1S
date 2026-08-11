#pragma once

#include <Arduino.h>

enum LogLevel : uint8_t {
  LOG_DEBUG = 0,
  LOG_INFO = 1,
  LOG_WARNING = 2,
  LOG_ERROR = 3
};

void logMessage(LogLevel level, const char *module, const String &message);
void logInfo(const char *module, const String &message);
void logWarn(const char *module, const String &message);
void logError(const char *module, const String &message);
