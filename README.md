# DODO PRINT - Bambu P1S Desktop Monitor

Monitor de escritorio read-only para una Bambu Lab P1S usando un ESP32 con pantalla TFT ST7789 de 1.9 pulgadas.

La pantalla del modulo es `170x320` nativa. El proyecto la usa en horizontal como `320x170`, con la orientacion fisica definitiva:

```text
LANDSCAPE / USB EN EL LADO IZQUIERDO
```

## Estado Actual

Incluye una arquitectura funcional y modular con:

- `DEMO_MODE` activo por defecto.
- Display real ST7789 activo por defecto.
- Modelo central `PrinterState`.
- Wi-Fi no bloqueante.
- Cliente MQTT local read-only.
- Parser JSON tolerante a reportes parciales.
- UI dashboard adaptada a `320x170`.
- Pantalla AMS secundaria preparada.
- Documentacion MQTT y hardware separada.

El firmware no publica comandos MQTT ni contiene funciones para pausar, reanudar, cancelar, calentar, mover ejes, extruir o controlar AMS.

## Hardware Identificado

Producto:

```text
ideaspark ESP32 Development Board 16MB Integrated 1.9 inch ST7789
Modelo: ESP32 1.9 inch LCD(Solder PIN)
ASIN: B0D6QXC813
USB: Type-C con CH340
Display: ST7789 170x320
Flash: 16 MB
Touch: no documentado para esta variante
```

Pinout LCD configurado:

```text
MOSI / SDA  GPIO23
SCLK        GPIO18
CS          GPIO15
DC          GPIO2
RST         GPIO4
BL / BLK    GPIO32
```

## Arquitectura

```text
Bambu Lab P1S
  -> Wi-Fi / LAN local
  -> MQTT TLS local
  -> ESP32
  -> Display ST7789 320x170 landscape
```

El estado fluye asi:

```text
MQTT -> bambu_parser -> PrinterState -> UI -> DisplayDriver
```

## Arbol De Archivos

```text
platformio.ini
README.md
docs/
  BAMBU_MQTT.md
  HARDWARE_IDEASPARK_ESP32_1_9_ST7789.md
include/
  config.h
  config.example.h
src/
  main.cpp
  wifi_manager.cpp/.h
  bambu_mqtt.cpp/.h
  bambu_parser.cpp/.h
  printer_state.cpp/.h
  display_driver.cpp/.h
  ui.cpp/.h
  ui_dashboard.cpp/.h
  ui_ams.cpp/.h
  demo_data.cpp/.h
  time_utils.cpp/.h
  logger.cpp/.h
```

## Configuracion

Edita `include/config.h`.

Valores principales:

```cpp
#define WIFI_SSID "YOUR_WIFI_SSID"
#define WIFI_PASSWORD "YOUR_WIFI_PASSWORD"
#define PRINTER_IP "192.168.1.100"
#define PRINTER_SERIAL "YOUR_PRINTER_SERIAL"
#define PRINTER_ACCESS_CODE "YOUR_LAN_ACCESS_CODE"
```

Pantalla:

```cpp
#define DISPLAY_CONTROLLER "ST7789"
#define DISPLAY_NATIVE_WIDTH 170
#define DISPLAY_NATIVE_HEIGHT 320
#define DISPLAY_WIDTH 320
#define DISPLAY_HEIGHT 170

// LANDSCAPE ORIENTATION - USB CONNECTOR ON LEFT
#define DISPLAY_ROTATION 1
#define DISPLAY_INVERT_COLORS 0

#define TFT_MOSI_PIN 23
#define TFT_SCLK_PIN 18
#define TFT_CS_PIN 15
#define TFT_DC_PIN 2
#define TFT_RST_PIN 4
#define TFT_BL_PIN 32
```

`DISPLAY_ROTATION` esta en `1`, valor usado en ejemplos del mismo modulo para landscape. Si al verlo de frente con USB a la izquierda queda invertido, cambia a `3`.

## Modo Demo

Por defecto:

```cpp
#define DEMO_MODE 1
#define USE_REAL_DISPLAY 1
```

Esto no conecta a la impresora. Simula:

- Proyecto: `Chimuelo - Llaveros`
- Progreso aproximado: `68%`
- Restante: `00:47`
- Capa: `184/270`
- Nozzle: `220/220C`
- Cama: `55/55C`
- AMS: `PLA NEGRO`

Con `USE_REAL_DISPLAY 1`, la UI sale en la pantalla integrada. Si necesitas probar sin pantalla, cambia temporalmente a `0` y veras salida por Serial.

## Compilar

Instala PlatformIO y ejecuta:

```bash
pio run
```

En este equipo tambien puedes usar la ruta instalada por Python:

```powershell
& "$env:APPDATA\Python\Python312\Scripts\pio.exe" run
```

El proyecto ya fue verificado con PlatformIO:

```text
SUCCESS
RAM:   8.2%
Flash: 17.4%
```

## Subir Al ESP32

Conecta la placa por USB-C. Debe aparecer un puerto CH340, por ejemplo `COM6`.

```bash
pio run --target upload --upload-port COM6
pio device monitor --port COM6
```

Velocidad Serial:

```text
115200
```

Durante esta revision solo aparecieron puertos Bluetooth (`COM3`, `COM4`, `COM5`), no el CH340 de la placa, asi que no se subio firmware.

## Activar MQTT Real

En `include/config.h`:

```cpp
#define DEMO_MODE 0
```

Configura Wi-Fi, IP local de la P1S, serial y LAN Access Code.

El monitor se conecta al broker local de la impresora, se suscribe a `device/<serial>/report` y solamente recibe datos.

## Display Real

El display esta implementado con:

```text
Adafruit_GFX
Adafruit_ST7789
SPI
```

Si la pantalla enciende pero no muestra imagen:

1. Verifica que `TFT_BL_PIN` sea `32`.
2. Prueba `DISPLAY_ROTATION 3`.
3. Prueba `DISPLAY_INVERT_COLORS 1` si los colores se ven invertidos.
4. Evita presionar el boton conectado a `GPIO4`, porque ese pin coincide con `RST` del display.

## Prueba Recomendada

1. Compila con `DEMO_MODE 1`.
2. Sube el firmware.
3. Verifica que aparece `DODO PRINT` y los datos demo en la pantalla.
4. Si el texto queda al reves, cambia `DISPLAY_ROTATION` de `1` a `3`.
5. Configura credenciales reales.
6. Cambia `DEMO_MODE 0`.
7. Comprueba logs Wi-Fi.
8. Comprueba logs MQTT.
9. Verifica que `printerConnected`, `wifiConnected` y `mqttConnected` se muestran por separado.

## Problemas Frecuentes

Pantalla invertida:
Cambia `DISPLAY_ROTATION` de `1` a `3`.

Colores invertidos:
Cambia `DISPLAY_INVERT_COLORS` de `0` a `1`.

Backlight apagado:
Verifica `TFT_BL_PIN 32`.

No conecta MQTT:
Verifica IP, serial, LAN Access Code, puerto `8883`, usuario `bblp` y que la P1S este en la misma LAN.

Conecta Wi-Fi pero no P1S:
La impresora puede estar apagada, en otra VLAN, con IP distinta o bloqueada por firewall.

Datos incompletos:
Los reportes MQTT pueden ser parciales. El parser conserva valores anteriores cuando un campo no llega.

## Fuentes

MQTT:
[docs/BAMBU_MQTT.md](docs/BAMBU_MQTT.md)

Hardware:
[docs/HARDWARE_IDEASPARK_ESP32_1_9_ST7789.md](docs/HARDWARE_IDEASPARK_ESP32_1_9_ST7789.md)
