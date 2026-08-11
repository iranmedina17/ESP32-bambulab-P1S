#include "logger.h"
#include "config.h"

static const char *levelName(LogLevel level) {
  switch (level) {
    case LOG_DEBUG: return "DEBUG";
    case LOG_INFO: return "INFO";
    case LOG_WARNING: return "WARN";
    case LOG_ERROR: return "ERROR";
  }
  return "INFO";
}

void logMessage(LogLevel level, const char *module, const String &message) {
  if (level < LOG_LEVEL) return;
  Serial.print('[');
  Serial.print(levelName(level));
  Serial.print("][");
  Serial.print(module);
  Serial.print("] ");
  Serial.println(message);
}

void logInfo(const char *module, const String &message) { logMessage(LOG_INFO, module, message); }
void logWarn(const char *module, const String &message) { logMessage(LOG_WARNING, module, message); }
void logError(const char *module, const String &message) { logMessage(LOG_ERROR, module, message); }
