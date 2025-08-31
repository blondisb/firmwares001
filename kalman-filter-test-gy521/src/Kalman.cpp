#include "Kalman.h"

Kalman::Kalman() {}

void Kalman::setAngle(double a) { angle = a; }

double Kalman::getAngle(double newAngle, double newRate, double dt) {
    rate = newRate - bias;
    angle += dt * rate;

    // Estimation error covariance
    P[0][0] += dt * (dt*P[1][1] - P[0][1] - P[1][0] + Q_angle);
    P[0][1] -= dt * P[1][1];
    P[1][0] -= dt * P[1][1];
    P[1][1] += Q_bias * dt;

    double S = P[0][0] + R_measure;
    double K[2];
    K[0] = P[0][0]/S;
    K[1] = P[1][0]/S;

    double y = newAngle - angle;
    angle += K[0] * y;
    bias += K[1] * y;

    double P00_temp = P[0][0];
    double P01_temp = P[0][1];

    P[0][0] -= K[0] * P00_temp;
    P[0][1] -= K[0] * P01_temp;
    P[1][0] -= K[1] * P00_temp;
    P[1][1] -= K[1] * P01_temp;

    return angle;
}