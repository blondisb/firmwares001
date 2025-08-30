#include <Arduino.h>
#include "GPSModule.h"

void setup() {
  Serial.begin(115200);
  GPS_begin();
  Serial.println("Iniciando programa principal...");
}

void loop() {
  GPS_update();  // siempre alimentar

  static unsigned long lastCheck = 0;
  if (millis() - lastCheck > 2000) {
    lastCheck = millis();
    double lat, lng;
    if (GPS_getLocation(lat, lng)) {
      Serial.print("Lat: ");
      Serial.print(lat, 6);
      Serial.print(", Lng: ");
      Serial.println(lng, 6);
    } else {
      Serial.println("Sin fix todavía...");
    }
  }
}
