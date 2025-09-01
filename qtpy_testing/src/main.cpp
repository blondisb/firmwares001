#include <Adafruit_NeoPixel.h>

#define PIN_NEOPIXEL 8   // Pin del NeoPixel integrado en QT Py M0
Adafruit_NeoPixel pixel(1, PIN_NEOPIXEL, NEO_GRB + NEO_KHZ800);

void setup() {
  Serial.begin(115200);
  while (!Serial) delay(10);
  Serial.println("Encendiendo NeoPixel en color fijo...");

  pixel.begin();
  pixel.setBrightness(150);  // ajusta brillo (0-255)

  // Enciende en rojo
  pixel.setPixelColor(0, pixel.Color(255, 0, 0));
  pixel.show();
}

void loop() {
  // Nada que hacer aquí, el LED queda fijo
}
