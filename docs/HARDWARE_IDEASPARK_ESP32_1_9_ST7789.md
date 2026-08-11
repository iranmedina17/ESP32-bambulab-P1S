# ideaspark ESP32 1.9 inch LCD(Solder PIN)

Hardware objetivo de este proyecto:

```text
Marca: ideaspark
Modelo: ESP32 1.9 inch LCD(Solder PIN)
ASIN: B0D6QXC813
Pantalla: ST7789 170x320
Flash anunciada: 16 MB
USB: Type-C, CH340
Touch: no documentado para esta variante
```

## Pinout LCD

```text
LCD signal   ESP32 GPIO
MOSI/SDA     GPIO23
SCLK         GPIO18
CS           GPIO15
DC           GPIO2
RST          GPIO4
BL/BLK       GPIO32
```

## Configuracion Del Proyecto

```cpp
#define DISPLAY_CONTROLLER "ST7789"
#define DISPLAY_NATIVE_WIDTH 170
#define DISPLAY_NATIVE_HEIGHT 320
#define DISPLAY_WIDTH 320
#define DISPLAY_HEIGHT 170
#define DISPLAY_ROTATION 1

#define TFT_MOSI_PIN 23
#define TFT_SCLK_PIN 18
#define TFT_CS_PIN 15
#define TFT_DC_PIN 2
#define TFT_RST_PIN 4
#define TFT_BL_PIN 32
```

## Libreria

Se usa `Adafruit_ST7789` porque hay ejemplos comunitarios directos con este modulo y estos pines.

Inicializacion:

```cpp
SPI.begin(18, -1, 23, 15);
tft.init(170, 320);
tft.setRotation(1);
```

## Notas

- `GPIO4` aparece relacionado con reset del LCD en reportes comunitarios. Evita usarlo como boton general.
- Si la pantalla queda en blanco tras presionar un boton conectado a `GPIO4`, reinicia la placa.
- Si el texto queda al reves con USB a la izquierda, usa `DISPLAY_ROTATION 3`.
- Si los colores quedan raros, prueba `DISPLAY_INVERT_COLORS 1`.

## Fuentes

- Amazon B0D6QXC813: producto ideaspark ESP32 1.9 inch LCD.
- Discusion Tasmota sobre ideaspark ESP32 ST7789 170x320: https://github.com/arendst/Tasmota/discussions/22669
- Arduino Forum con ejemplo Adafruit_ST7789 y pines del modulo: https://forum.arduino.cc/t/esp32-development-board-1-9in/1405296
