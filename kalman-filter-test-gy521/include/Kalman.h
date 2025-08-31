#ifndef KALMAN_H
#define KALMAN_H

class Kalman {
public:
    Kalman();
    double getAngle(double newAngle, double newRate, double dt);
    void setAngle(double angle);
private:
    double Q_angle = 0.001;
    double Q_bias = 0.003;
    double R_measure = 0.03;

    double angle = 0.0;
    double bias = 0.0;
    double rate = 0.0;

    double P[2][2] = {{0, 0}, {0, 0}};
};

#endif
