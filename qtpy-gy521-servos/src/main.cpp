#include <Wire.h>
#include <Adafruit_NeoPixel.h>
#include <Servo.h>


float RateRoll, RatePitch, RateYaw;
float RateCalibrationRoll, RateCalibrationPitch, RateCalibrationYaw;
int RateCalibrationNumber;

float AccX, AccY, AccZ;
float AngleRoll, AnglePitch;
uint32_t LoopTimer;

float AccZInertial;
float VelocityVertical;


//----------------------------------------------------------------------------
Servo servoPitch; // timón de cola
Servo servoRoll;  // alerones
// --- PID Pitch ---
float setpointPitch = 0;   // objetivo (ej. nivelado)
float KpPitch = 3.0, KiPitch = 0.01, KdPitch = 1.5;
float errorPitch, prevErrorPitch = 0, integralPitch = 0;
// --- PID Roll ---
float setpointRoll = 0;
float KpRoll = 3.0, KiRoll = 0.01, KdRoll = 1.5;
float errorRoll, prevErrorRoll = 0, integralRoll = 0;
// --- Timer para deltaT ---
unsigned long prevTime;
//----------------------------------------------------------------------------




//Predictes angles and uncertainty 
float KalmanAngleRoll=0, KalmanUncertaintyAngleRoll=2*2;
float KalmanAnglePitch=0, KalmanUncertaintyAnglePitch=2*2;
float KalmanAngleYaw=0, KalmanUncertaintyAngleYaw=2*2;

//Initialize output filter {anglePrediction, Uncertainty of the Prediction}
float Kalman1DOutput[] = {0,0};

//Predicted angle by kalman filter
void kalman_1d(
  float KalmanState,
  float KalmanUncertainty,
  float KalmanInput,
  float KalmanMeasurement
){
  KalmanState = KalmanState + 0.004*KalmanInput;
  KalmanUncertainty = KalmanUncertainty + 0.004*4*4;
  float KalmanGain = KalmanUncertainty * 1/(1*KalmanUncertainty + 3*3);
  KalmanState = KalmanState + KalmanGain * (KalmanMeasurement - KalmanState);
  KalmanUncertainty = (1-KalmanGain) * KalmanUncertainty;
  Kalman1DOutput[0] = KalmanState;
  Kalman1DOutput[1] = KalmanUncertainty;
}

void gyro_signals(void){
  Wire.beginTransmission(0x68);
  Wire.write(0x1A);
  Wire.write(0x05);
  Wire.endTransmission();
  // Acc outputs
  Wire.beginTransmission(0x68);
  Wire.write(0x1C);
  Wire.write(0x10);
  Wire.endTransmission();
  //Pulling acceleration data:
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

void setup(){
  Serial.begin(57600);
  Wire.setClock(400000);
  Wire.begin();
  delay(250);
  //
  Wire.beginTransmission(0x68);
  Wire.write(0x6B);
  Wire.write(0x00);
  Wire.endTransmission();

  //
  for(
    RateCalibrationNumber=0;
    RateCalibrationNumber<2000;
    RateCalibrationNumber++
  ){
    gyro_signals();
    RateCalibrationRoll+=RateRoll;
    RateCalibrationPitch+=RatePitch;
    RateCalibrationYaw+=RateYaw;
    delay(1);
  }

  RateCalibrationRoll/=2000;
  RateCalibrationPitch/=2000;
  RateCalibrationYaw/=2000;
  LoopTimer=micros();

  // Iniciar servos
  servoPitch.attach(7); // Pin D5 (puedes cambiarlo)
  servoRoll.attach(8);  // Pin D6 (puedes cambiarlo)
  servoPitch.write(90);
  servoRoll.write(90);
  prevTime = millis();
}


void loop(){
  //
  gyro_signals();

  AccZInertial = -sin(AnglePitch*(3.142/180))*AccX + cos(AnglePitch*(3.142/180))*sin(AngleRoll*(3.142/180))*AccY + cos(AnglePitch*(3.142/180))*cos(AngleRoll*(3.142/180))*AccZ;
  AccZInertial = (AccZInertial - 1)*9.81*100; //Removing gravity and converting to cm/s^2
  VelocityVertical += AccZInertial*0.004; //Vertical velocity in cm/s

  RateRoll -= RateCalibrationRoll;
  RatePitch -= RateCalibrationPitch;
  RateYaw -= RateCalibrationYaw;

  // Startting Kalman for roll filtering
  kalman_1d( KalmanAngleRoll, KalmanUncertaintyAngleRoll, RateRoll, AngleRoll);
  KalmanAngleRoll = Kalman1DOutput[0];
  KalmanUncertaintyAngleRoll = Kalman1DOutput[1];
  
  // Startting Kalman for pitch filtering
  kalman_1d( KalmanAnglePitch, KalmanUncertaintyAnglePitch, RatePitch, AnglePitch);
  KalmanAnglePitch = Kalman1DOutput[0];
  KalmanUncertaintyAnglePitch = Kalman1DOutput[1];

  
  // --- Delta tiempo ---
  unsigned long now = millis();
  float dt = (now - prevTime) / 1000.0;
  prevTime = now;

  // --- Control PID Pitch ---
  errorPitch = setpointPitch - KalmanAnglePitch;
  integralPitch += errorPitch * dt;
  float derivativePitch = (errorPitch - prevErrorPitch) / dt;
  float outputPitch = KpPitch * errorPitch + KiPitch * integralPitch + KdPitch * derivativePitch;
  prevErrorPitch = errorPitch;

  // --- Control PID Roll ---
  errorRoll = setpointRoll - KalmanAngleRoll;
  integralRoll += errorRoll * dt;
  float derivativeRoll = (errorRoll - prevErrorRoll) / dt;
  float outputRoll = KpRoll * errorRoll + KiRoll * integralRoll + KdRoll * derivativeRoll;
  prevErrorRoll = errorRoll;

  // --- Mover servos ---
  int servoPitchPos = constrain(90 + outputPitch, 0, 180);
  int servoRollPos = constrain(90 + outputRoll, 0, 180);
  servoPitch.write(servoPitchPos);
  servoRoll.write(servoRollPos);

  Serial.print("signal1= ");
  Serial.print(KalmanAnglePitch);
  Serial.print(" \t signal2= ");
  Serial.print(servoPitchPos);
  Serial.print(" \t signal3= ");
  Serial.print(0);
  Serial.print(" \t signal4= ");
  Serial.print(0); //Removing initial offset
  Serial.print(" \t signal5= ");
  Serial.println(0);

  delay(10); // Pequeña pausa para estabilidad
  
  // while (micros() - LoopTimer < 4000){
  //   LoopTimer = micros();
  // }
}




