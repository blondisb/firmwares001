#include <Arduino.h>
#include <TinyGPS++.h>

#include <SoftwareSerial.h>

// Pines para comunicación GPS
#define RXPin 4  // GPS TX -> Arduino RX
#define TXPin 3  // GPS RX -> Arduino TX

SoftwareSerial gpsSerial(RXPin, TXPin);
TinyGPSPlus gps;

void setup() {
  Serial.begin(115200);       // Monitor Serial
  gpsSerial.begin(9600);    // Velocidad típica del Quectel L86
  Serial.println("Iniciando GPS Quectel L86...");
}

void loop() {
  while (gpsSerial.available() > 0) {
    char c = gpsSerial.read();

    Serial.write(c);
    
    if (gps.encode(c)){  // Procesa los datos NMEA
      Serial.print("Latitud: ");
      Serial.println(gps.location.lat(), 6);

      Serial.print("Longitud: ");
      Serial.println(gps.location.lng(), 6);

      Serial.print("Satélites: ");
      Serial.println(gps.satellites.value());

      Serial.print("Altitud (m): ");
      Serial.println(gps.altitude.meters());

      Serial.print("Velocidad (km/h): ");
      Serial.println(gps.speed.kmph());

      char buffer[20];
      sprintf(buffer, "%02d:%02d:%02d",
                  gps.time.hour(),
                  gps.time.minute(),
                  gps.time.second());
      Serial.print("Hora UTC: ");
      Serial.println(buffer);
    }
    //delay(1000);
    
  }
}
