#include <Arduino.h>

int motorPin = 9;         // PWM -> Gate MOSFET
int motor2Pin = 10;         // PWM -> Gate MOSFET
int ledPin   = LED_BUILTIN; // LED interno del Arduino

void setup() {
  pinMode(motorPin, OUTPUT);
  pinMode(ledPin, OUTPUT);
}

void loop() {
  // Velocidad baja (~20%)
  analogWrite(motorPin, 50);
  analogWrite(motor2Pin, 50);
  digitalWrite(ledPin, HIGH);   // LED encendido
  delay(2000);
  digitalWrite(ledPin, LOW);    // LED apagado
  delay(2000);

  // Velocidad media (~50%)
  analogWrite(motorPin, 128);
  analogWrite(motor2Pin, 250);
  digitalWrite(ledPin, HIGH);
  delay(2000);
  digitalWrite(ledPin, LOW);
  delay(2000);

  // Velocidad alta (~80%)
  analogWrite(motorPin, 200);
  analogWrite(motor2Pin, 200);
  digitalWrite(ledPin, HIGH);
  delay(2000);
  digitalWrite(ledPin, LOW);
  delay(2000);

  // Máxima velocidad (100%)
  analogWrite(motorPin, 255);
  analogWrite(motor2Pin, 128);
  digitalWrite(ledPin, HIGH);
  delay(2000);
  digitalWrite(ledPin, LOW);
  delay(2000);

  // Motor apagado
  analogWrite(motorPin, 0);
  analogWrite(motor2Pin, 0);
  digitalWrite(ledPin, HIGH);
  delay(2000);
  digitalWrite(ledPin, LOW);
  delay(2000);
}
