#include "bambu_parser.h"
#include <ArduinoJson.h>
#include "logger.h"

static String readString(JsonObject obj, const char *key, const String &fallback) {
  return obj[key].is<const char *>() ? String(obj[key].as<const char *>()) : fallback;
}

static int readInt(JsonObject obj, const char *key, int fallback) {
  return obj[key].is<int>() ? obj[key].as<int>() : fallback;
}

static float readFloat(JsonObject obj, const char *key, float fallback) {
  return obj[key].is<float>() || obj[key].is<int>() ? obj[key].as<float>() : fallback;
}

static void parseAms(JsonObject print, PrinterState &state) {
  if (!print["ams"].is<JsonObject>()) return;
  JsonObject ams = print["ams"].as<JsonObject>();
  state.amsConnected = true;
  state.activeAms = readInt(ams, "ams_id", state.activeAms);
  state.activeTray = readInt(ams, "tray_now", state.activeTray);

  if (!ams["tray"].is<JsonArray>()) return;
  JsonArray trays = ams["tray"].as<JsonArray>();
  int index = 0;
  for (JsonObject tray : trays) {
    if (index >= 4) break;
    state.trays[index].present = true;
    state.trays[index].type = readString(tray, "tray_type", state.trays[index].type);
    state.trays[index].color = readString(tray, "tray_color", state.trays[index].color);
    state.trays[index].name = readString(tray, "tray_sub_brands", state.trays[index].name);
    state.trays[index].active = (index == state.activeTray);
    if (state.trays[index].active) {
      state.filamentType = state.trays[index].type;
      state.filamentColor = state.trays[index].color;
    }
    index++;
  }
}

bool parsePrinterReport(const uint8_t *payload, size_t length, PrinterState &state) {
  JsonDocument doc;
  DeserializationError error = deserializeJson(doc, payload, length);
  if (error) {
    logWarn("MQTT", String("JSON parse failed: ") + error.c_str());
    return false;
  }

  JsonObject root = doc.as<JsonObject>();
  JsonObject print = root["print"].is<JsonObject>() ? root["print"].as<JsonObject>() : root;
  if (print.isNull()) return false;

  state.lastPrinterMessageMs = millis();
  state.printerConnected = true;

  state.projectName = readString(print, "subtask_name", state.projectName);
  state.filename = readString(print, "gcode_file", state.filename);
  if (!state.projectName.length()) state.projectName = state.filename;

  state.rawState = readString(print, "gcode_state", state.rawState);
  state.stage = readString(print, "mc_print_stage", state.stage);
  state.progress = readInt(print, "mc_percent", state.progress);
  state.remainingMinutes = readInt(print, "mc_remaining_time", state.remainingMinutes);
  state.currentLayer = readInt(print, "layer_num", state.currentLayer);
  state.totalLayers = readInt(print, "total_layer_num", state.totalLayers);

  state.nozzleTemp = readFloat(print, "nozzle_temper", state.nozzleTemp);
  state.nozzleTarget = readFloat(print, "nozzle_target_temper", state.nozzleTarget);
  state.bedTemp = readFloat(print, "bed_temper", state.bedTemp);
  state.bedTarget = readFloat(print, "bed_target_temper", state.bedTarget);
  state.chamberTemp = readFloat(print, "chamber_temper", state.chamberTemp);

  state.speedMode = readString(print, "spd_lvl", state.speedMode);
  state.errorCode = readString(print, "print_error", state.errorCode);
  state.hmsCode = readString(print, "hms", state.hmsCode);
  state.errorMessage = readString(print, "msg", state.errorMessage);

  parseAms(print, state);
  return true;
}
