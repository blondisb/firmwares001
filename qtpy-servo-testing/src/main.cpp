#include <Servo.h>
#include <Adafruit_NeoPixel.h>

// --- Configuración ---
#define SERVO_PIN 7       // Pin PWM para el servo
#define NEOPIXEL_PIN 11   // Pin de datos NeoPixel en QT Py (default es 11)
#define NUMPIXELS 60       // Usamos un solo NeoPixel

Servo myServo;
Adafruit_NeoPixel pixels(NUMPIXELS, NEOPIXEL_PIN, NEO_GRB + NEO_KHZ800);

// --- Colores extremos (RGB) ---
uint8_t color0[3] = {255, 0, 0};   // Rojo para 0°
uint8_t color180[3] = {0, 0, 255}; // Azul para 180°

void setup() {
  myServo.attach(SERVO_PIN);
  myServo.write(90); // posición inicial en centro

  pixels.begin();
  pixels.setPixelColor(0, pixels.Color(255, 0, 0));
  pixels.setBrightness(100); // empieza apagado.
  pixels.show();

  delay(500);
}

void loop() {
  // --- Movimiento de 0° a 180° ---
  for (int pos = 0; pos <= 180; pos++) {
    moverServoYLeds(pos);
    delay(20); // velocidad del barrido
  }

  // --- Movimiento de 180° a 0° ---
  for (int pos = 180; pos >= 0; pos--) {
    moverServoYLeds(pos);
    delay(20);
  }
}

// --- Función que sincroniza servo + color ---
void moverServoYLeds(int pos) {
  // Mueve servo
  myServo.write(pos);

  // Interpolación lineal de color entre color0 y color180
  uint8_t r = map(pos, 0, 180, color0[0], color180[0]);
  uint8_t g = map(pos, 0, 180, color0[1], color180[1]);
  uint8_t b = map(pos, 0, 180, color0[2], color180[2]);

  // Brillo también en función de pos (de 0 en 0° hasta 255 en 180°)
  uint8_t brightness = map(pos, 0, 180, 0, 255);

  pixels.setBrightness(brightness);
  pixels.setPixelColor(0, pixels.Color(r, g, b));
  pixels.show();
}
