// #define LED_BUILTIN 16   // LED del RP2040 Zero

// #include <Arduino.h>

// void setup() {
//   pinMode(LED_BUILTIN, OUTPUT);
//   Serial.begin(115200);   // ahora sí debe funcionar con USB
//   while (!Serial) delay(10);  
//   Serial.println("Hola desde RP2040 Zero!");
// }

// void loop() {
//   digitalWrite(LED_BUILTIN, HIGH);
//   delay(500);
//   digitalWrite(LED_BUILTIN, LOW);
//   delay(500);
//   Serial.println("Hola desde RP2040 Zero!");
// }


#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

#define LED_PIN   16    // GPIO16 en la RP2040-Zero
#define LED_COUNT 1     // Solo 1 Neopixel en la placa

Adafruit_NeoPixel pixels(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  Serial.begin(115200);   // ahora sí debe funcionar con USB
  Serial.println("Hola desde RP2040 Zero!");
  pixels.begin();       // Inicializa el Neopixel
  pixels.setBrightness(50); // brillo de 0 a 255
}

void loop() {
  // Rojo
  pixels.setPixelColor(0, pixels.Color(255, 0, 0));
  pixels.show();
  Serial.println("Color Rojo");
  delay(700);
  

  // Verde
  pixels.setPixelColor(0, pixels.Color(0, 255, 0));
  pixels.show();
  Serial.println("Color Verde");
  delay(700);
  

  // Azul
  pixels.setPixelColor(0, pixels.Color(0, 0, 255));
  pixels.show();
  Serial.println("Color Azul");
  delay(700);

  // yellow
  pixels.setPixelColor(0, pixels.Color(255, 150, 0));
  pixels.show();
  Serial.println("Color Amarillo");
  delay(700);

  // purple
  pixels.setPixelColor(0, pixels.Color(180, 0, 255)); 
  pixels.show();
  Serial.println("Color Morado");
  delay(700);

  // cyan
  pixels.setPixelColor(0, pixels.Color(0, 255, 255));
  pixels.show();
  Serial.println("Color Cyan");
  delay(700);


  // Apagado
  pixels.clear();
  pixels.show();
  Serial.println("-----");
  delay(500);
}
