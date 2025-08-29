#include <Arduino.h>

void setup() {
  // Start serial communication
  Serial.begin(9600);
}

void loop() {
  // Read analog values from A0 to A3
  int dimmer1 = analogRead(A0);
  int dimmer2 = analogRead(A1);
  int dimmer3 = analogRead(A2);
  int dimmer4 = analogRead(A3);

  // Print the values to the serial monitor
  Serial.print("Dimmer 1: ");
  Serial.print(dimmer1);
  Serial.print("\tDimmer 2: ");
  Serial.print(dimmer2);
  Serial.print("\tDimmer 3: ");
  Serial.print(dimmer3);
  Serial.print("\tDimmer 4: ");
  Serial.println(dimmer4);

  // Wait 500 milliseconds before next read
  delay(500);
}
