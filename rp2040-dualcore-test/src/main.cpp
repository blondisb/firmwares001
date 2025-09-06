#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include "pico/multicore.h"

// --- Configuración NeoPixel integrado ---
#define PIN_NEOPIXEL 16   // RP2040 Zero -> pin 16
#define NUMPIXELS    1

Adafruit_NeoPixel pixels(NUMPIXELS, PIN_NEOPIXEL, NEO_GRB + NEO_KHZ800);

// --- Función pesada (benchmark simple) ---
uint32_t fibonacci(uint32_t n) {
  if (n < 2) return n;
  return fibonacci(n - 1) + fibonacci(n - 2);
}

// --- Tarea que correrá en el Core1 ---
void core1_entry() {
  while (true) {
    // Alterna color del NeoPixel cada 500 ms
    pixels.setPixelColor(0, pixels.Color(150, 255, 50)); // 
    pixels.show();
    Serial.println("-------Color 1");
    sleep_ms(500);

    pixels.setPixelColor(0, pixels.Color(255, 50, 150)); //
    pixels.show();
    Serial.println("-------Color 2");
    sleep_ms(500);
  }
}

void setup() {
  Serial.begin(115200);   // Usa Serial porque Serial USB a veces no aparece
  pixels.begin();

  Serial.println("Iniciando benchmark dual-core...");

  // Lanzamos tarea en el segundo núcleo
  multicore_launch_core1(core1_entry);
}

void loop() {
  // Core 0 hace benchmark de Fibonacci
  for (int i = 30; i < 36; i++) {
    uint32_t start = millis();
    uint32_t result = fibonacci(i);
    uint32_t elapsed = millis() - start;

    Serial.print("Fib(");
    Serial.print(i);
    Serial.print(") = ");
    Serial.print(result);
    Serial.print(" | tiempo = ");
    Serial.print(elapsed);
    Serial.println(" ms");
  }
  delay(2000);
}
