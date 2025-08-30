#include <Arduino.h>
#include <TinyGPS++.h>
#include <SoftwareSerial.h>

// Pines de RX/TX del GPS
static const int RXPin = 4, TXPin = 3;
static const uint32_t GPSBaud = 9600;

// Objetos globales internos al módulo
SoftwareSerial gpsSerial(RXPin, TXPin);
TinyGPSPlus gps;

// Inicialización del GPS
void GPS_begin() {
  gpsSerial.begin(GPSBaud);
}

// Este se llama frecuentemente en loop() para alimentar al GPS
void GPS_update() {
  while (gpsSerial.available() > 0) {
    char c = gpsSerial.read();
    gps.encode(c); // procesa el carácter
  }
}

// Devuelve true si hay una nueva localización válida
bool GPS_getLocation(double &lat, double &lng) {
  if (gps.location.isUpdated()) {
    lat = gps.location.lat();
    lng = gps.location.lng();
    return true;
  }
  return false;
}

// Devuelve true si hay una nueva hora válida
bool GPS_getTime(int &h, int &m, int &s) {
  if (gps.time.isUpdated()) {
    h = gps.time.hour();
    m = gps.time.minute();
    s = gps.time.second();
    return true;
  }
  return false;
}
