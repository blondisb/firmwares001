#ifndef GPSMODULE_H
#define GPSMODULE_H

void GPS_begin();
void GPS_update();
bool GPS_getLocation(double &lat, double &lng);
bool GPS_getTime(int &h, int &m, int &s);

#endif