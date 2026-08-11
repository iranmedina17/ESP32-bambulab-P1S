# DODO PRINT - Bambu P1S Desktop Monitor

Public documentation package for publishing the project on GitHub.

This folder intentionally does not contain private data such as Wi-Fi password, printer LAN access code, printer IP, or printer serial number.

---

# Espanol

## Que Es

**DODO PRINT** es un monitor de escritorio read-only para una impresora **Bambu Lab P1S**, usando un ESP32 con pantalla **ST7789 1.9 inch 170x320** en orientacion horizontal.

El monitor muestra informacion rapida:

- Nombre del proyecto.
- Barra de avance.
- Porcentaje.
- Tiempo restante.
- Temperaturas.
- Estado Wi-Fi / MQTT / P1S.

La comunicacion con la impresora se hace por **MQTT local/LAN**, sin depender de Bambu Cloud para la lectura de datos.

## Seguridad

Esta version es **solo lectura**.

El firmware:

- No pausa impresiones.
- No reanuda impresiones.
- No cancela impresiones.
- No mueve ejes.
- No cambia temperaturas.
- No extruye filamento.
- No envia G-code.
- No controla AMS.
- No publica comandos MQTT operativos.

Solo se suscribe al topic de reportes de la impresora.

## Hardware Usado

Modulo probado:

```text
ideaspark ESP32 1.9 inch LCD(Solder PIN)
ASIN: B0D6QXC813
Pantalla: ST7789 170x320
USB: Type-C CH340
Flash: 16 MB anunciados
```

Pinout LCD:

```text
MOSI / SDA  GPIO23
SCLK        GPIO18
CS          GPIO15
DC          GPIO2
RST         GPIO4
BL / BLK    GPIO32
```

Orientacion fisica:

```text
Landscape / horizontal
USB connector on left
```

## Archivos Que Debes Configurar

Copia:

```text
include/config.example.h
```

como:

```text
include/config.h
```

Luego edita `include/config.h`.

Importante: `include/config.h` contiene datos privados y debe estar en `.gitignore`.

## Datos Necesarios

### 1. Nombre Del Wi-Fi

Campo:

```cpp
#define WIFI_SSID "YOUR_WIFI_SSID"
```

Donde conseguirlo:

- En el router.
- En la configuracion Wi-Fi de tu computadora o telefono.
- Debe ser la misma red local donde esta conectada la Bambu P1S.

### 2. Clave Del Wi-Fi

Campo:

```cpp
#define WIFI_PASSWORD "YOUR_WIFI_PASSWORD"
```

Donde conseguirla:

- En la etiqueta del router.
- En el panel de administracion del router.
- En tu administrador de contrasenas.

No subas este dato a GitHub.

### 3. IP Local De La Impresora

Campo:

```cpp
#define PRINTER_IP "192.168.x.x"
```

Donde conseguirla:

- Pantalla de la Bambu P1S: ajustes de red / WLAN / IP.
- App Bambu Handy: informacion del dispositivo.
- Bambu Studio: detalles del dispositivo.
- Lista de clientes conectados en el router.

Recomendacion:

Configura una reserva DHCP en tu router para que la impresora conserve siempre la misma IP.

### 4. Serial De La Impresora

Campo:

```cpp
#define PRINTER_SERIAL "YOUR_PRINTER_SERIAL"
```

Donde conseguirlo:

- Pantalla de la impresora: ajustes / informacion del dispositivo.
- Bambu Studio: Device / Printer details.
- Bambu Handy: configuracion de la impresora / informacion.
- Etiqueta fisica de la impresora.

El serial se usa para suscribirse al topic:

```text
device/<PRINTER_SERIAL>/report
```

No es la direccion MAC.

### 5. LAN Access Code

Campo:

```cpp
#define PRINTER_ACCESS_CODE "YOUR_LAN_ACCESS_CODE"
```

Donde conseguirlo:

- Pantalla de la Bambu P1S.
- Menu de red / WLAN / LAN mode.
- En algunas versiones aparece como "Access Code" o "Codigo de acceso".

No subas este dato a GitHub.

### 6. Modo Demo

Campo:

```cpp
#define DEMO_MODE 1
```

Usa `1` para probar la pantalla sin conectarte a la impresora.

Usa `0` para conectarte a la impresora real.

## Compilar

Con PlatformIO:

```bash
pio run
```

## Subir Firmware

Primero identifica el puerto:

```bash
pio device list
```

Ejemplo de puerto CH340:

```text
COM9
```

Subir:

```bash
pio run --target upload --upload-port COM9
```

Monitor Serial:

```bash
pio device monitor --port COM9 --baud 115200
```

## Logs Esperados

Cuando todo funciona:

```text
[INFO][WIFI] Connected
[INFO][MQTT] Subscribed to device/<serial>/report
[INFO][MQTT] Report received from P1S
```

## Problemas Comunes

Pantalla al reves:

```cpp
#define DISPLAY_ROTATION 3
```

o vuelve a:

```cpp
#define DISPLAY_ROTATION 1
```

Colores raros:

```cpp
#define DISPLAY_INVERT_COLORS 1
```

o:

```cpp
#define DISPLAY_INVERT_COLORS 0
```

MQTT no conecta:

- Verifica IP de la impresora.
- Verifica serial.
- Verifica LAN Access Code.
- Verifica que el ESP32 y la P1S esten en la misma red local.
- Verifica que la impresora tenga LAN mode disponible/activo.

---

# English

## What It Is

**DODO PRINT** is a read-only desktop monitor for a **Bambu Lab P1S** printer, built with an ESP32 and a **1.9 inch ST7789 170x320** display in landscape orientation.

It shows quick-glance information:

- Project name.
- Progress bar.
- Percentage.
- Remaining time.
- Temperatures.
- Wi-Fi / MQTT / P1S status.

Printer communication uses **local/LAN MQTT**, avoiding Bambu Cloud dependency for status monitoring.

## Safety

This version is **read-only**.

The firmware:

- Does not pause prints.
- Does not resume prints.
- Does not cancel prints.
- Does not move axes.
- Does not change temperatures.
- Does not extrude filament.
- Does not send G-code.
- Does not control AMS.
- Does not publish operational MQTT commands.

It only subscribes to the printer report topic.

## Tested Hardware

Tested module:

```text
ideaspark ESP32 1.9 inch LCD(Solder PIN)
ASIN: B0D6QXC813
Display: ST7789 170x320
USB: Type-C CH340
Advertised flash: 16 MB
```

LCD pinout:

```text
MOSI / SDA  GPIO23
SCLK        GPIO18
CS          GPIO15
DC          GPIO2
RST         GPIO4
BL / BLK    GPIO32
```

Physical orientation:

```text
Landscape / horizontal
USB connector on left
```

## Files To Configure

Copy:

```text
include/config.example.h
```

to:

```text
include/config.h
```

Then edit `include/config.h`.

Important: `include/config.h` contains private data and must stay in `.gitignore`.

## Required Data

### 1. Wi-Fi Name

Field:

```cpp
#define WIFI_SSID "YOUR_WIFI_SSID"
```

Where to find it:

- Router settings.
- Wi-Fi settings on your computer or phone.
- It must be the same local network used by the Bambu P1S.

### 2. Wi-Fi Password

Field:

```cpp
#define WIFI_PASSWORD "YOUR_WIFI_PASSWORD"
```

Where to find it:

- Router label.
- Router admin panel.
- Password manager.

Do not upload this to GitHub.

### 3. Printer Local IP

Field:

```cpp
#define PRINTER_IP "192.168.x.x"
```

Where to find it:

- Bambu P1S screen: network / WLAN / IP.
- Bambu Handy app: device information.
- Bambu Studio: device details.
- Router connected-client list.

Recommendation:

Create a DHCP reservation in your router so the printer keeps the same IP.

### 4. Printer Serial Number

Field:

```cpp
#define PRINTER_SERIAL "YOUR_PRINTER_SERIAL"
```

Where to find it:

- Printer screen: settings / device information.
- Bambu Studio: Device / Printer details.
- Bambu Handy: printer settings / information.
- Physical printer label.

The serial is used to subscribe to:

```text
device/<PRINTER_SERIAL>/report
```

It is not the MAC address.

### 5. LAN Access Code

Field:

```cpp
#define PRINTER_ACCESS_CODE "YOUR_LAN_ACCESS_CODE"
```

Where to find it:

- Bambu P1S screen.
- Network / WLAN / LAN mode menu.
- On some firmware versions it appears as "Access Code".

Do not upload this to GitHub.

### 6. Demo Mode

Field:

```cpp
#define DEMO_MODE 1
```

Use `1` to test the display without connecting to the printer.

Use `0` to connect to the real printer.

## Build

With PlatformIO:

```bash
pio run
```

## Upload Firmware

First identify the serial port:

```bash
pio device list
```

Example CH340 port:

```text
COM9
```

Upload:

```bash
pio run --target upload --upload-port COM9
```

Serial Monitor:

```bash
pio device monitor --port COM9 --baud 115200
```

## Expected Logs

When everything works:

```text
[INFO][WIFI] Connected
[INFO][MQTT] Subscribed to device/<serial>/report
[INFO][MQTT] Report received from P1S
```

## Common Issues

Display upside down:

```cpp
#define DISPLAY_ROTATION 3
```

or back to:

```cpp
#define DISPLAY_ROTATION 1
```

Wrong colors:

```cpp
#define DISPLAY_INVERT_COLORS 1
```

or:

```cpp
#define DISPLAY_INVERT_COLORS 0
```

MQTT does not connect:

- Verify printer IP.
- Verify serial number.
- Verify LAN Access Code.
- Verify ESP32 and P1S are on the same local network.
- Verify LAN mode is available/enabled on the printer.
