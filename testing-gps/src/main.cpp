#include <Arduino.h>
#include <SoftwareSerial.h>

// Pines para comunicación GPS
#define RXPin 4  // GPS TX -> Arduino RX
#define TXPin 3  // GPS RX -> Arduino TX

SoftwareSerial gpsSerial(RXPin, TXPin);

void setup() {
  Serial.begin(9600);       // Monitor Serial
  gpsSerial.begin(9600);    // Velocidad típica del Quectel L86
  Serial.println("Iniciando GPS Quectel L86...");
}

void loop() {
  while (gpsSerial.available() > 0) {
    char c = gpsSerial.read();
    Serial.write(c);  // Imprime las tramas NMEA en el monitor serie
  }
}