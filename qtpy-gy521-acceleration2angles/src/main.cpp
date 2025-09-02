#include <Wire.h>
#include <Adafruit_NeoPixel.h>

float RateRoll, RatePitch, RateYaw;
float AccX, AccY, AccZ;
float AngleRoll, AnglePitch;
float LoopTimer;

void gyro_signals(void){
  Wire.beginTransmission(0x68);
  Wire.write(0x1A);
  Wire.write(0x05);
  Wire.endTransmission();
  //Acc outputs:
  Wire.beginTransmission(0x68);
  Wire.write(0x1C); // this is where acc is stored in module mem
  Wire.write(0x10); // picking 8g scale
  Wire.endTransmission();
  // Pulling acc data:
  Wire.beginTransmission(0x68);
  Wire.write(0x3B);
  Wire.endTransmission();
  Wire.requestFrom(0x68,6);
  int16_t AccXLSB = Wire.read() << 8 | Wire.read();
  int16_t AccYLSB = Wire.read() << 8 | Wire.read();
  int16_t AccZLSB = Wire.read() << 8 | Wire.read();
  // Pulling gyro data:
  Wire.beginTransmission(0x68);
  Wire.write(0x1B);
  Wire.write(0x8);
  Wire.endTransmission();
  //
  Wire.beginTransmission(0x68);
  Wire.write(0x43);
  Wire.endTransmission();
  Wire.requestFrom(0x68,6);
  int16_t GyroX = Wire.read() <<8 | Wire.read();
  int16_t GyroY = Wire.read() <<8 | Wire.read();
  int16_t GyroZ = Wire.read() <<8 | Wire.read();
  //
  RateRoll =  (float)GyroX/65.5; //el numero viene del datasheet
  RatePitch = (float)GyroY/65.5;
  RateYaw =   (float)GyroZ/65.5;
  // Converting acc from LSB to G, (follow up datasheet):
  AccX = (float)AccXLSB/4096 - 0.03;
  AccY = (float)AccYLSB/4096 - 0.02;
  AccZ = (float)AccZLSB/4096 + 0.01;
  //--
  //Calculating Angles, remeber pitagoras:
  AngleRoll   = atan(AccY / sqrt(AccX*AccX + AccZ*AccZ)) * 1/(3.142/180);
  AnglePitch  = -atan(AccX / sqrt(AccY*AccY + AccZ*AccZ)) * 1/(3.142/180);
}

void setup() {
  Serial.begin(57600);
  Wire.setClock(400000);
  Wire.begin();
  delay(250);
  Wire.beginTransmission(0x68);
  Wire.write(0x6B);
  Wire.write(0x00);
  Wire.endTransmission();
}

void loop(){
  gyro_signals();

  Serial.print("signal1= ");
  Serial.print(AngleRoll);
  Serial.print(" \t signal2= ");
  Serial.print(0);
  Serial.print(" \t signal3= ");
  Serial.print(RateRoll);
  Serial.print(" \t signal4= ");
  Serial.print(0);
  Serial.print(" \t signal5= ");
  Serial.println(0);
  delay(50);
}