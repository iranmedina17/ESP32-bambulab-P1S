#pragma once

#include <Arduino.h>
#include "printer_state.h"

bool parsePrinterReport(const uint8_t *payload, size_t length, PrinterState &state);
