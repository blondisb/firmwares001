#include "MPU6050.h"
#include <Wire.h>

#define MPU_ADDR 0x68

MPU6050::MPU6050() {
    accX = accY = accZ = 0;
    gyroX = gyroY = gyroZ = 0;
    roll = pitch = yaw = 0;
}

void MPU6050::begin() {
    Wire.begin();
    Wire.beginTransmission(MPU_ADDR);
    Wire.write(0x6B); 
    Wire.write(0); // Despertar MPU6050
    Wire.endTransmission(true);
    timer = micros();
}

void MPU6050::readRawData() {
    Wire.beginTransmission(MPU_ADDR);
    Wire.write(0x3B);
    Wire.endTransmission(false);
    Wire.requestFrom(MPU_ADDR, 14, true);

    accX = Wire.read()<<8 | Wire.read();
    accY = Wire.read()<<8 | Wire.read();
    accZ = Wire.read()<<8 | Wire.read();
    int16_t temp = Wire.read()<<8 | Wire.read(); // temperatura
    gyroX = Wire.read()<<8 | Wire.read();
    gyroY = Wire.read()<<8 | Wire.read();
    gyroZ = Wire.read()<<8 | Wire.read();
}

void MPU6050::update() {
    readRawData();

    float dt = (micros() - timer) / 1000000.0;
    timer = micros();

    // Conversión acelerómetro a ángulos
    float rollAcc  = atan2(accY, accZ) * 180/PI;
    float pitchAcc = atan(-accX / sqrt(accY*accY + accZ*accZ)) * 180/PI;

    roll += gyroX * dt / 131.0;
    pitch += gyroY * dt / 131.0;
    yaw += gyroZ * dt / 131.0;

    roll = rollAcc; // Para simplificación (o usar Kalman)
    pitch = pitchAcc;
}
