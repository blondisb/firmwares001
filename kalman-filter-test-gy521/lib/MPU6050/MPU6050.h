#ifndef MPU6050_H
#define MPU6050_H

#include <Arduino.h>
#include <Wire.h>

class MPU6050 {
public:
    MPU6050();
    void begin();
    void update();
    // Datos crudos
    int16_t accX, accY, accZ;
    int16_t gyroX, gyroY, gyroZ;
    // Ángulos calculados
    float roll, pitch, yaw;
private:
    void readRawData();
    long timer;
};

#endif