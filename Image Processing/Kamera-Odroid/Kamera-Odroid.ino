

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
//#include <Pixy.h>
#include <SoftwareSerial.h>
#include <TimerOne.h>
#include <Wire.h>
#include <HMC5883L.h>
#include <math.h>
#include "compass.h"
//#define PI 3.14159265
//#define X_CENTER        ((PIXY_MAX_X-PIXY_MIN_X)/2)       
//#define Y_CENTER        ((PIXY_MAX_Y-PIXY_MIN_Y)/2)
#define SOP 'x'
#define EOP 'y'
  
float derajat;
char flag;

//Pixy pixy;
//SoftwareSerial Serial2(10,11);
HMC5883L compass;
Vector norm;
uint8_t ball = 0;
uint8_t readdata;
int a=0;

  
//search for ball


//class ServoLoop
//{
//public:
//  ServoLoop(int32_t pgain, int32_t dgain);
//
//  void update(int32_t error);
//   
//  int32_t m_pos;
//  int32_t m_prevError;
//  int32_t m_pgain;
//  int32_t m_dgain;
//};


//ServoLoop panLoop(300, 500);
//ServoLoop tiltLoop(300, 500);
//
//ServoLoop::ServoLoop(int32_t pgain, int32_t dgain)
//{
//  m_pos = PIXY_RCS_CENTER_POS;
//  m_pgain = pgain;
//  m_dgain = dgain;
//  m_prevError = 0x80000000L;
//}

//void ServoLoop::update(int32_t error)
//{
//  long int vel;
//  char buf[32];
//  if (m_prevError!=0x80000000)
//  {	
//    vel = (error*m_pgain + (error - m_prevError)*m_dgain)>>10;
//    //sprintf(buf, "%ld\n", vel);
//    //Serial.print(buf);
//    m_pos += vel;
//    if (m_pos>PIXY_RCS_MAX_POS) 
//      m_pos = PIXY_RCS_MAX_POS; 
//    else if (m_pos<PIXY_RCS_MIN_POS) 
//      m_pos = PIXY_RCS_MIN_POS;
//  }
//  m_prevError = error;
//}


bool getball(){
  int i;
   uint16_t blocks;
  for(i=0;i<30;i++){
//    blocks = pixy.getBlocks();
    if(blocks){
    return true;
    }
  }

  return false;
}

//void toSerial(){
//  while(Serial2.available()==!0){
//    Serial.write(Serial2.read());
//  }
//  Serial2.write("a");
//}

void compassSetup(){
  Wire.begin();
  compass_x_offset = 291.67;
  compass_y_offset = 118.69;
  compass_z_offset = -227.39;
  compass_x_gainError = 1.12;
  compass_y_gainError = 1.13;
  compass_z_gainError = 1.03;
  
  compass_init(2);
}

void compassLoop(){
   compass_scalled_reading();
   compass_heading();
}

void interruptHandler(){
//  Serial.println(bearing);
  uint8_t xCoor;
  uint8_t yCoor;
  uint8_t heading;
  char sendBuffer[8];
  uint8_t ballXCoor=0;
  uint8_t ballYCoor=0;

    //compassLoop();
    heading = bearing*0.7083;
    //sprintf(ui,"%d",a);
    
  if(flag=='X'){
    xCoor = readdata;
    flag = 0;
  }
  else if(flag=='Y'){
    yCoor = readdata;
    flag = 0;
  }
  else
  {
    flag = readdata;
  }
    sendBuffer[0] = 'X';
    sendBuffer[1] = xCoor;
//    sendBuffer[1] = ballXCoor;
    sendBuffer[2] = 'Y';
    sendBuffer[3] = yCoor;
//    sendBuffer[3] = 5;//ballYCoor;
    sendBuffer[4] = 'C';
    sendBuffer[5] = heading;
    sendBuffer[6] = 'B';
    sendBuffer[7] = ball;
    Serial2.write(sendBuffer[0]);
    Serial2.write(sendBuffer[1]);
    Serial2.write(sendBuffer[2]);
    Serial2.write(sendBuffer[3]);
    Serial2.write(sendBuffer[4]);
    Serial2.write(sendBuffer[5]);
    Serial2.write(sendBuffer[6]);
    Serial2.write(sendBuffer[7]);
//    establishContact();
//    Serial.write(sendBuffer[0]);
//    Serial.write(sendBuffer[1]);
//    Serial.write(sendBuffer[2]);
//    Serial.write(sendBuffer[3]);
//    Serial.write(sendBuffer[4]);
//    Serial.write(sendBuffer[5]);
//    Serial.write(sendBuffer[6]);
//    Serial.write(sendBuffer[7]);  
   
}

void setup()
{
  
  Serial2.begin(9600);
  Serial1.begin(9600);
  Serial.begin(9600);
  Serial.print("Starting...\n");
//  compassSetup();
  Timer1.initialize(50000); 
  Timer1.attachInterrupt(interruptHandler);

//  pixy.init();
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
  while(1){
    counter++;
    if(counter == 2000){
//      compassLoop();
      //Serial.println(bearing);
      counter =0;
    }
    else{}  
//    blocks = pixy.getBlocks();
    if(Serial1.available()){
    readdata = Serial1.read();
    }
    
    else{}
    if(Serial.available()){
    Serial.write(Serial.read());
    }
    else{}
    
    if (blocks)
    {
      ball = 1;
      counterBallDetection = 0;
//      panError =pixy.blocks[0].x- X_CENTER;
//      tiltError = Y_CENTER-pixy.blocks[0].y;
//      
//      panLoop.update(panError);
//      tiltLoop.update(tiltError);
//      if(tiltLoop.m_pos>PIXY_RCS_MAX_POS-300){
//        tiltLoop.m_pos =PIXY_RCS_MAX_POS-300;
//      }
//      pixy.setServos(panLoop.m_pos, tiltLoop.m_pos);
    }
    else{
     counterBallDetection ++;
     if(counterBallDetection == 2000){
//         tiltLoop.m_pos =300;
//         panLoop.m_pos =PIXY_RCS_CENTER_POS;
//         pixy.setServos(panLoop.m_pos, tiltLoop.m_pos);
         counterBallDetection = 0;
         ball = 0;
      } 
    }
  }
  
}

void establishContact() {
  while (Serial.available() <= 0) {
    Serial.write(Serial.read()); 
  }
}




