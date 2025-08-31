#include <Arduino.h>
#include <MPU6050.h>
#include <Wire.h>

MPU6050 mpu;

const int motorIzq = 9;
const int motorDer = 10;
unsigned long startTime;

// PID
double setpoint = 0;    // Ángulo objetivo (equilibrio)
double input, output;
double error, lastError = 0, integral = 0;

// Ganancias PID (ajustar por prueba y error)
double Kp = 20;
double Ki = 0.5;
double Kd = 5;

void setup() {
  Serial.begin(9600);
  Wire.begin();
  mpu.initialize();

  pinMode(motorIzq, OUTPUT);
  pinMode(motorDer, OUTPUT);

  if (!mpu.testConnection()) {
    Serial.println("Error: MPU6050 no conectado!");
    while (1);
  }
  Serial.println("MPU6050 listo");
  startTime = millis();
}

void loop() {
  // Leer ángulo con MPU (ejemplo eje X como balanceo)
  int16_t ax, ay, az, gx, gy, gz;
  mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

  // Calcular inclinación aproximada (solo eje X)
  input = atan2(ay, az) * 180 / PI;

  // Calcular PID
  error = setpoint - input;
  integral += error;
  double derivative = error - lastError;
  output = Kp * error + Ki * integral + Kd * derivative;
  lastError = error;

  // Mapear salida PID a rango PWM (0–255)
  int pwmIzq = constrain(150 + output, 0, 255);
  int pwmDer = constrain(150 - output, 0, 255);

  if (millis()-startTime < 10000){
    analogWrite(motorIzq, pwmIzq);
    analogWrite(motorDer, pwmDer);
  }else{
    analogWrite(motorIzq, 0);
    analogWrite(motorDer, 0);
  }

  // Debug
  Serial.print("|\tAngX: "); Serial.print(input);
  Serial.print("|\tPWM_Izq: "); Serial.print(pwmIzq);
  Serial.print("|\tPWM_Der: "); Serial.println(pwmDer);
  Serial.print("|\tAngY: "); Serial.print(atan2(ax, az) * 180 / PI);
  Serial.print("|\tAngZ: "); Serial.print(atan2(ay, ax) * 180 / PI);

  delay(10);
}
