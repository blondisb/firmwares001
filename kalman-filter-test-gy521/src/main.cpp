#include <Arduino.h>
#include "MPU6050.h"
#include "Kalman.h"

MPU6050 mpu;
Kalman kalmanX, kalmanY;

void setup() {
    Serial.begin(115200);
    mpu.begin();
    delay(100);
    kalmanX.setAngle(0);
    kalmanY.setAngle(0);
}

void loop() {
    mpu.update();

    float dt = 0.01; // aprox 10ms loop
    float roll = kalmanX.getAngle(mpu.roll, mpu.gyroX/131.0, dt);
    float pitch = kalmanY.getAngle(mpu.pitch, mpu.gyroY/131.0, dt);

    Serial.print("Acc X: "); Serial.print(mpu.accX);
    Serial.print(" Y: "); Serial.print(mpu.accY);
    Serial.print(" Z: "); Serial.print(mpu.accZ);
    Serial.print(" | Gyro X: "); Serial.print(mpu.gyroX);
    Serial.print(" Y: "); Serial.print(mpu.gyroY);
    Serial.print(" Z: "); Serial.print(mpu.gyroZ);
    Serial.print(" | Roll: "); Serial.print(roll);
    Serial.print(" Pitch: "); Serial.println(pitch);

    delay(10);
}
