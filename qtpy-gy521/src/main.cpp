#include <Wire.h>
#include <Adafruit_NeoPixel.h>

// put function declarations here:
float RateRoll, RatePitch, RateYaw;
float RateCalibrationRoll, RateCalibrationPitch, RateCalibrationYaw;
int RateCalibrationNumber;

void gyro_signals(void){
  Wire.beginTransmission(0x68);
  Wire.write(0x1A);
  Wire.write(0x05);
  Wire.endTransmission();
  //.
  Wire.beginTransmission(0x68);
  Wire.write(0x1B);
  Wire.write(0x8);
  Wire.endTransmission();
  //.
  Wire.beginTransmission(0x68);
  Wire.write(0x43);
  Wire.endTransmission();
  Wire.requestFrom(0x68,6);
  //.
  int16_t GyroX = Wire.read() <<8 | Wire.read();
  int16_t GyroY = Wire.read() <<8 | Wire.read();
  int16_t GyroZ = Wire.read() <<8 | Wire.read(); 
  //.
  RateRoll = (float)GyroX/65.5; //el numero viene del datasheet
  RatePitch = (float)GyroY/65.5;
  RateYaw = (float)GyroZ/65.5;
}

// the data pin for the NeoPixels
int neoPixelPin = 11;
int numPixels = 60;
Adafruit_NeoPixel strip = Adafruit_NeoPixel(numPixels, neoPixelPin, NEO_GRB + NEO_KHZ800);
int r = 0;
int g = 0;
int b = 0;

char buffer1[100];

void setup() {
  strip.begin();  // initialize the strip
  strip.show();   // make sure it is visible
  strip.clear();  // Initialize all pixels to 'off'
  //--
  Serial.begin(57600);
  strip.setBrightness(12.5);

  for( int i = 0; i < numPixels; i++ ){
    strip.setPixelColor(i, r, g, b);
  }
  
  Wire.setClock(400000);
  Wire.begin();
  delay(250);
  //-- Activate MPU6050
  Wire.beginTransmission(0x68);
  Wire.write(0x6B);
  Wire.write(0x00);
  Wire.endTransmission();
  //--
  for (RateCalibrationNumber = 0;
       RateCalibrationNumber < 2000;
       RateCalibrationNumber++ ){
        gyro_signals();
        RateCalibrationRoll += RateRoll;
        RateCalibrationPitch += RatePitch;
        RateCalibrationYaw += RateYaw;
        delay(1);
  }
  RateCalibrationRoll/=2000;
  RateCalibrationPitch/=2000;
  RateCalibrationYaw/=2000;
}

void loop() {
  // put your main code here, to run repeatedly:
  gyro_signals();
  //.
  RatePitch -= RateCalibrationPitch;
  RateRoll -= RateCalibrationRoll;
  RateYaw -= RateCalibrationYaw;
  //.
  Serial.print("roll_rate ");
  Serial.print(RateRoll);
  Serial.print(" \t pitch_rate ");
  Serial.print(RatePitch);
  Serial.print(" \t yaw_rate ");
  Serial.println(RateYaw);


  if (RateRoll > 100) r = 255;
  else if (RateRoll < -100) r= 0;

  if (RatePitch > 100) g = 255;
  else if (RatePitch < -100) g= 0;

  if (RateYaw > 100) b = 255;
  else if (RateYaw < -100) b= 0;

  for( int i = 0; i < numPixels; i++ ){
    strip.setPixelColor(i, r, g, b);
  }
  strip.show();
  delay(50);
}
