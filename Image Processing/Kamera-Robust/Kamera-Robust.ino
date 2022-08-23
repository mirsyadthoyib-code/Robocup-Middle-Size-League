

//
// begin license header
//
// This file is part of Pixy CMUcam5 or "Pixy" for short
//
// All Pixy source code is provided under the terms of the
// GNU General Pu blic License v2 (http://www.gnu.org/licenses/gpl-2.0.html).
// Those wishing to use Pixy source code, software and/or
// technologies under different licensing terms should contact us at
// cmucam@cs.cmu.edu. Such licensing terms are available for
// all portions of the Pixy codebase presented here.
//
// end license header
//
// This sketch is a simple tracking demo that uses the pan/tilt unit.  For
// more information, go here:
//
// http://cmucam.org/projects/cmucam5/wiki/Run_the_Pantilt_Demo
//

#include <SPI.h>
#include <Pixy.h>
#include <SoftwareSerial.h>
#include <TimerOne.h>
#include <Wire.h>
#include <HMC5883L.h>
#include <math.h>
#include "compass.h"
#include <Wire.h>
#include "Kalman.h" // Source: https://github.com/TKJElectronics/KalmanFilter
#define PI 3.14159265
#define X_CENTER        ((PIXY_MAX_X-PIXY_MIN_X)/2)
#define Y_CENTER        ((PIXY_MAX_Y-PIXY_MIN_Y)/2)
#define Address 0x60

Kalman kalmanX;
Kalman kalmanY;
Kalman kalmanZ;
Kalman kalmank;
Kalman kalmanK;

double zeroValue[5] = { -200, 44, 660, 52.3, -18.5}; // Found by experimenting

/* All the angles start at 180 degrees */
double gyroXangle = 180;
double gyroYangle = 180;
double gyroZangle = 180;

double compAngleX = 180;
double compAngleY = 180;
double compAngleZ = 180;

double angleHigh = 0;

unsigned long timer;

uint8_t buffer[2]; // I2C buffer

float derajat;
float anglefix;

Pixy pixy;
SoftwareSerial stmSerial(5, 6);
HMC5883L compass;
Vector norm;
uint8_t ball = 0;
//search for ball


class ServoLoop
{
  public:
    ServoLoop(int32_t pgain, int32_t dgain);

    void update(int32_t error);

    int32_t m_pos;
    int32_t m_prevError;
    int32_t m_pgain;
    int32_t m_dgain;
};


ServoLoop panLoop(300, 500);
ServoLoop tiltLoop(300, 500);

ServoLoop::ServoLoop(int32_t pgain, int32_t dgain)
{
  m_pos = PIXY_RCS_CENTER_POS;
  m_pgain = pgain;
  m_dgain = dgain;
  m_prevError = 0x80000000L;
}

void ServoLoop::update(int32_t error)
{
  long int vel;
  char buf[32];
  if (m_prevError != 0x80000000)
  {
    vel = (error * m_pgain + (error - m_prevError) * m_dgain) >> 10;
    //sprintf(buf, "%ld\n", vel);
    //Serial.print(buf);
    m_pos += vel;
    if (m_pos > PIXY_RCS_MAX_POS)
      m_pos = PIXY_RCS_MAX_POS;
    else if (m_pos < PIXY_RCS_MIN_POS)
      m_pos = PIXY_RCS_MIN_POS;
  }
  m_prevError = error;
}


bool getball() {
  int i;
  uint16_t blocks;
  for (i = 0; i < 30; i++) {
    blocks = pixy.getBlocks();
    if (blocks) {
      return true;
    }
  }

  return false;
}

void toSerial() {
  while (stmSerial.available() == !0) {
    Serial.write(stmSerial.read());
  }
  stmSerial.write("a");
}

void compassSetup() {
  Wire.begin();
  compass_x_offset = 291.67;
  compass_y_offset = 118.69;
  compass_z_offset = -227.39;
  compass_x_gainError = 1.12;
  compass_y_gainError = 1.13;
  compass_z_gainError = 1.03;

  compass_init(2);
}

void compassLoop() {
  compass_scalled_reading();
  compass_heading();
}

void i2cWrite(uint8_t address, uint8_t registerAddress) {
  Wire.beginTransmission(address);
  Wire.write(registerAddress);
  Wire.endTransmission();
}

uint8_t* i2cRead(uint8_t address, uint8_t registerAddress, uint8_t nbytes) {
  Wire.beginTransmission(address);
  Wire.write(registerAddress);
  Wire.endTransmission();
  Wire.beginTransmission(address);
  Wire.requestFrom(address, nbytes);
  for (uint8_t i = 0; i < nbytes; i++)
    buffer[i] = Wire.read();
  Wire.endTransmission();
  return buffer;
}

int readGyroX() { // This really measures the y-axis of the gyro
  uint8_t* data = i2cRead(Address, 18, 2);
  return ((data[0] << 8) + data[1]);
}

int readGyroY() { // This really measures the x-axis of the gyro
  uint8_t* data = i2cRead(Address, 20, 2);
  return ((data[0] << 8) + data[1]);
}

int readGyroZ() { // This really measures the x-axis of the gyro
  uint8_t* data = i2cRead(Address, 22, 2);
  return ((data[0] << 8) + data[1]);
}

double getXangle() {
  double accXval = (double)readAccX(); - zeroValue[0];
  double accZval = (double)readAccZ(); - zeroValue[2];
  double angle = (atan2(accXval, accZval) + PI) * RAD_TO_DEG;
  return angle;
}

double getYangle() {
  double accYval = (double)readAccY(); - zeroValue[1];
  double accZval = (double)readAccZ(); - zeroValue[2];
  double angle = (atan2(accYval, accZval) + PI) * RAD_TO_DEG;
  return angle;
}

double getZangle() {
  double accXval = (double)readAccX(); - zeroValue[0];
  double accYval = (double)readAccY(); - zeroValue[1];
  double angle = (atan2(accXval, accYval) + PI) * RAD_TO_DEG;
  return angle;
}

double getAngle() {
  uint8_t* data = i2cRead(Address, 1, 1);
  return (data[0] * 1.4117647058823529411764705882353);
}

double getAngleHigh() {
  uint8_t* data = i2cRead(Address, 2, 2);
  return (((data[0] << 8) + data[1]) / 10);
}

int readAccX() {
  uint8_t* data = i2cRead(Address, 6, 2);
  return ((data[0] << 8) + data[1]);
}

int readAccY() {
  uint8_t* data = i2cRead(Address, 8, 2);
  return ((data[0] << 8) + data[1]);
}

int readAccZ() {
  uint8_t* data = i2cRead(Address, 10, 2);
  return ((data[0] << 8) + data[1]);
}

void cmpsSetup()
{
  kalmanX.setAngle(180); // Set starting angle
  kalmanY.setAngle(180);
  kalmanZ.setAngle(180);
  kalmank.setAngle(180);
  kalmanK.setAngle(180);
  timer = micros();
}

void cmpsLoop()
{
  double gyroXrate = -(((double)readGyroX() /*- zeroValue[3]*/) / 14.375);
  gyroXangle += gyroXrate * ((double)(micros() - timer) / 1000000); // Without any filter

  double gyroYrate = (((double)readGyroY() /*- zeroValue[4]*/) / 14.375);
  gyroYangle += gyroYrate * ((double)(micros() - timer) / 1000000); // Without any filter

  double gyroZrate = (((double)readGyroZ() /*- zeroValue[4]*/) / 14.375);
  gyroZangle += gyroZrate * ((double)(micros() - timer) / 1000000); // Without any filter

  double accXangle = getXangle();
  double accYangle = getYangle();
  double accZangle = getZangle();
  double acckangle = getAngle();
  double accKangle = getAngleHigh();

  compAngleX = (0.93 * (compAngleX + (gyroXrate * (double)(micros() - timer) / 1000000))) + (0.07 * accXangle);
  compAngleY = (0.93 * (compAngleY + (gyroYrate * (double)(micros() - timer) / 1000000))) + (0.07 * accYangle);
  compAngleZ = (0.93 * (compAngleZ + (gyroZrate * (double)(micros() - timer) / 1000000))) + (0.07 * accZangle);

  double xAngle = kalmanX.getAngle(accXangle, gyroXrate, (double)(micros() - timer)); // calculate the angle using a Kalman filter
  double yAngle = kalmanY.getAngle(accYangle, gyroYrate, (double)(micros() - timer)); // calculate the angle using a Kalman filter
  double zAngle = kalmanZ.getAngle(accZangle, gyroZrate, (double)(micros() - timer)); // calculate the angle using a Kalman filter
  double kAngle = kalmank.getAngle(acckangle, gyroZrate, (double)(micros() - timer)); // calculate the angle using a Kalman filter
  double KAngle = kalmanK.getAngle(accKangle, gyroZrate, (double)(micros() - timer)); // calculate the angle using a Kalman filter

  timer = micros();

  /* print data to processing */
  //  Serial.print(gyroXangle); Serial.print("\t");
  //  Serial.print(gyroYangle); Serial.print("\t");
  angleHigh = getAngleHigh();

  if ((angleHigh >= 0) && (angleHigh < 96))
  {
    anglefix = map(angleHigh, 0, 95, 0, 90);
  }
  else if ((angleHigh >= 96) && (angleHigh < 161))
  {
    anglefix = map(angleHigh, 96, 160, 90, 180);
  }
  else if ((angleHigh >= 161) && (angleHigh < 271))
  {
    anglefix = map(angleHigh, 161, 270, 180, 270);
  }
  else if ((angleHigh >= 270) && (angleHigh < 360))
  {
    anglefix = map(angleHigh, 270, 360, 270, 360);
  }
  anglefix = map(anglefix, 0, 360, 0, 255);
}

void interruptHandler() {
  //  Serial.println(bearing);
  uint8_t xCoor;
  uint8_t yCoor;
  uint8_t heading;
  char sendBuffer[8];
  xCoor = (int)panLoop.m_pos / 4;
  yCoor = (int)tiltLoop.m_pos / 4;
//  heading = anglefix;
  heading = bearing*0.7083;
  sendBuffer[0] = 'X';
  sendBuffer[1] = xCoor;
  sendBuffer[2] = 'Y';
  sendBuffer[3] = yCoor;
  sendBuffer[4] = 'C';
  sendBuffer[5] = heading;
  sendBuffer[6] = 'B';
  sendBuffer[7] = ball;
//  Serial.println(heading);
  Serial.write(sendBuffer[0]);
  Serial.write(sendBuffer[1]);
  Serial.write(sendBuffer[2]);
  Serial.write(sendBuffer[3]);
  Serial.write(sendBuffer[4]);
  Serial.write(sendBuffer[5]);
  Serial.write(sendBuffer[6]);
  Serial.write(sendBuffer[7]);
}

void setup()
{
  Serial.begin(9600);
  Wire.begin();
  //stmSerial.begin(9600);
  Serial.print("Starting...\n");
  compassSetup();
//  cmpsSetup();
  Timer1.initialize(50000);
  Timer1.attachInterrupt(interruptHandler);

  pixy.init();
  tiltLoop.m_pos = 300;
  panLoop.m_pos = PIXY_RCS_CENTER_POS;
  pixy.setServos(panLoop.m_pos, tiltLoop.m_pos);
}

void loop()
{

  static int i = 0;
  int j;
  int counter;
  uint16_t blocks;
  char buf[32];
  char printSerial[32];
  int counterBallDetection;
  int32_t panError, tiltError;
  while (1) {
//    cmpsLoop();
    //    counter++;
    //    if(counter == 2000){
          compassLoop();
    //      //Serial.println(bearing);
    //      counter =0;
    //    }
    //    else{}
    blocks = pixy.getBlocks();
    if (blocks)
    {
      ball = 1;
      counterBallDetection = 0;
      panError = pixy.blocks[0].x - X_CENTER;
      tiltError = Y_CENTER - pixy.blocks[0].y;

      panLoop.update(panError);
      tiltLoop.update(tiltError);
      if (tiltLoop.m_pos > PIXY_RCS_MAX_POS - 300) {
        tiltLoop.m_pos = PIXY_RCS_MAX_POS - 300;
      }
      pixy.setServos(panLoop.m_pos, tiltLoop.m_pos);
    }
    else {
      counterBallDetection ++;
      if (counterBallDetection == 300) {
        tiltLoop.m_pos = 350;
        panLoop.m_pos = PIXY_RCS_CENTER_POS;
        pixy.setServos(panLoop.m_pos, tiltLoop.m_pos);
        counterBallDetection = 0;
        ball = 0;
      }
    }
  }
}

