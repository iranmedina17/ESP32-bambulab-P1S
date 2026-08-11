# Bambu Lab P1S MQTT LAN

Este documento resume la integración MQTT local usada por el monitor.

Bambu Lab no publica una especificación completa y estable del JSON MQTT local. Por eso el firmware trata todos los reportes como parciales, conserva valores anteriores y documenta los campos usados.

## Conexión Local

La información pública y comunitaria coincide en estos puntos para impresoras Bambu compatibles:

- Broker: IP local de la impresora.
- Puerto: `8883`.
- Transporte: MQTT sobre TLS.
- Usuario: `bblp`.
- Password: LAN Access Code de la impresora.
- Topic de reportes: `device/<PRINTER_SERIAL>/report`.

Fuentes:

- Foro Bambu Lab: ejemplo con `mosquitto_sub`, `8883`, certificado, usuario `bblp`, access code y topic wildcard: https://forum.bambulab.com/t/mqtt-support/58793
- Reddit Bambu Lab: `ipaddress:8883`, usuario `bblp`, password access code y topic `device/{serial}/report`: https://www.reddit.com/r/BambuLab/comments/13hsoq0/how_to_publish_mqtt_messages/
- Home Assistant Community: reportes sobre TLS, puerto `8883`, usuario `bblp` y access code: https://community.home-assistant.io/t/bambu-lab-x1-x1c-mqtt/489510
- OpenHAB Community: IP, serial y access code como datos necesarios: https://community.openhab.org/t/bambu-lab-3d-printer-integraton/146381

## Seguridad Read-Only

Este proyecto solo se suscribe a:

```text
device/<PRINTER_SERIAL>/report
```

No publica en topics de request y no implementa comandos.

## Certificado TLS

La P1S usa TLS local. En clientes de escritorio se puede extraer el certificado con `openssl`. En ESP32, esta primera versión usa:

```cpp
secureClient.setInsecure();
```

Eso evita validar el certificado en LAN y simplifica la primera conexión. Para endurecer seguridad, se puede almacenar el certificado de la impresora y usar `setCACert()`.

## Campos JSON Usados

El parser acepta mensajes con raíz:

```json
{
  "print": {}
}
```

También tolera mensajes donde los campos vienen directamente en la raíz.

Campos usados actualmente:

```text
subtask_name
gcode_file
gcode_state
mc_print_stage
mc_percent
mc_remaining_time
layer_num
total_layer_num
nozzle_temper
nozzle_target_temper
bed_temper
bed_target_temper
chamber_temper
spd_lvl
print_error
hms
msg
ams
```

Campos citados en fuentes comunitarias:

- `mc_percent`
- `mc_print_stage`
- `mc_remaining_time`
- `gcode_state`
- `mc_print_error_code`
- `layer_num`
- `total_layer_num`

Referencias:

- Home Assistant Community con ejemplos de campos `mc_percent`, `mc_print_stage`, `mc_print_error_code`: https://community.home-assistant.io/t/bambu-lab-x1-x1c-mqtt/489510/165
- Bambu Lab forum con `mc_percent`, `mc_print_stage`, `mc_remaining_time`, `gcode_start_timestamp`: https://forum.bambulab.com/t/a-way-to-document-printed-parts/4038
- Discusión sobre valores `gcode_state` como `RUNNING` y `FINISH`: https://forum.bambulab.com/t/x1c-mqtt-report-specification-eg-list-of-gcode-state-values/169693

## Estados Visuales

El firmware mapea:

```text
RUNNING/PRINTING -> IMPRIMIENDO
PAUSE/PAUSED     -> PAUSADA
FINISH/FINISHED  -> TERMINADA
PREPARE/SLICING  -> PREPARANDO
errores/HMS      -> ERROR
sin conexión     -> OFFLINE
default          -> LISTA
```

## AMS

El parser busca:

```text
ams.ams_id
ams.tray_now
ams.tray[].tray_type
ams.tray[].tray_color
ams.tray[].tray_sub_brands
```

La estructura interna soporta 4 slots en esta versión, con posibilidad de ampliarse a varios AMS.

## Limitaciones Conocidas

- Los nombres y valores exactos pueden variar por modelo y firmware.
- Algunos reportes pueden ser parciales.
- Algunos estados de finalización o error no siempre llegan de forma consistente según reportes comunitarios.
- La miniatura del proyecto no está implementada en esta versión.
- No se habilita ningún comando MQTT operativo.
