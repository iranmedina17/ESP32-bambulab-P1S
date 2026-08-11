#include "printer_state.h"
#include "config.h"

static bool equalsAny(const String &value, const char *a, const char *b = nullptr, const char *c = nullptr) {
  return value.equalsIgnoreCase(a) || (b && value.equalsIgnoreCase(b)) || (c && value.equalsIgnoreCase(c));
}

PrinterVisualState visualStateFor(const PrinterState &state) {
  if (!state.wifiConnected || !state.mqttConnected || !state.printerConnected) return PRINTER_OFFLINE;
  if (state.errorCode.length() || state.hmsCode.length()) return PRINTER_ERROR;
  if (equalsAny(state.rawState, "RUNNING", "PRINTING")) return PRINTER_PRINTING;
  if (equalsAny(state.rawState, "PAUSE", "PAUSED")) return PRINTER_PAUSED;
  if (equalsAny(state.rawState, "FINISH", "FINISHED")) return PRINTER_FINISHED;
  if (equalsAny(state.rawState, "PREPARE", "SLICING") || state.stage.length()) return PRINTER_PREPARING;
  return PRINTER_IDLE;
}

String visualStateText(PrinterVisualState state) {
  switch (state) {
    case PRINTER_IDLE: return "LISTA";
    case PRINTER_PREPARING: return "PREPARANDO";
    case PRINTER_PRINTING: return "IMPRIMIENDO";
    case PRINTER_PAUSED: return "PAUSADA";
    case PRINTER_FINISHED: return "TERMINADA";
    case PRINTER_ERROR: return "ERROR";
    case PRINTER_OFFLINE: return "OFFLINE";
  }
  return "UNKNOWN";
}

void updateDerivedTimes(PrinterState &state, uint32_t nowMs) {
  const PrinterVisualState visual = visualStateFor(state);
  if (visual == PRINTER_PRINTING && state.printStartedAtMs == 0) {
    state.printStartedAtMs = nowMs;
  }

  if ((visual == PRINTER_PRINTING || visual == PRINTER_PAUSED) && state.elapsedMinutes < 0 && state.printStartedAtMs > 0) {
    state.elapsedMinutes = static_cast<int>((nowMs - state.printStartedAtMs) / 60000UL);
  }

  if (state.remainingMinutes >= 0 && state.elapsedMinutes >= 0) {
    state.estimatedTotalMinutes = state.elapsedMinutes + state.remainingMinutes;
  }

  if (visual == PRINTER_FINISHED && state.finishedAtMs == 0) {
    state.finishedAtMs = nowMs;
  }

  if (visual == PRINTER_FINISHED && state.finishedAtMs > 0 && nowMs - state.finishedAtMs > FINISHED_SCREEN_DURATION_MS) {
    state.rawState = "IDLE";
    state.progress = 0;
    state.projectName = "";
    state.filename = "";
    state.finishedAtMs = 0;
    state.printStartedAtMs = 0;
  }
}

void updatePrinterTimeout(PrinterState &state, uint32_t nowMs) {
  if (state.lastPrinterMessageMs == 0) return;
  state.printerConnected = (nowMs - state.lastPrinterMessageMs) <= PRINTER_TIMEOUT_MS;
}
