#include <SoftwareSerial.h>
#include <Servo.h>

#define servoPinY 10
#define servoPinX 11

char data;
char flag;
uint8_t ball = 0;
uint8_t xCoor = 0;
uint8_t yCoor = 0;
uint8_t heading;
char sendBuffer[8];
int tiltLoop;
int panLoop;
int counterBallDetection = 0;
int k = 125, l = 75;
int prevxCoor = 0, prevyCoor = 0;
int tag = 0;

Servo ServoY;
Servo ServoX;
SoftwareSerial stmSerial(5, 6);

void setServo(int i, int j)
{
  while(k != j || l != i)
  {
    if(k != j)
    {
      panLoop = k;
      panLoop = map(panLoop, 0, 250, 0, 166);
      ServoX.write(panLoop);
      if(k > j)
      {
        k--;
      }
      else if(k < j)
      {
        k++;
      }
      else{}
      
      delay(5);
    }
    if(l != i)
    {
      tiltLoop = l;
      tiltLoop = map(tiltLoop, 0, 250, 0, 166);
      ServoY.write(tiltLoop);
      if(l > i)
      {
        l--;
      }
      else if(l < i)
      {
        l++;
      }
      else{}
      
      delay(5);
    }
  }
}

void setup() {
  Serial.begin(9600);
  stmSerial.begin(9600);
  ServoY.attach(servoPinY);
  ServoX.attach(servoPinX);
  setServo(125, 75);
}

void loop() 
{
  if(Serial.available())
  {
    data = Serial.read();
  }
  
  if(flag == 'X')
  {
    xCoor = data;
    flag = 0;
  }
  else if(flag == 'Y')
  {
    yCoor = data;
    flag = 0;
  }
  else
  {
    flag = data;
  }

  if(xCoor == 0 && yCoor == 0)
  {
    counterBallDetection++;
    if(counterBallDetection == 200)
    {
      xCoor = 125;
      yCoor = 75;
      if(xCoor != prevxCoor || yCoor != prevyCoor)
      {
        setServo(xCoor, yCoor);
        prevxCoor = xCoor;
        prevyCoor = yCoor;
      }
      else{}
      counterBallDetection = 0;
      ball = 0;
    }
  }
  else
  { 
    if(xCoor != prevxCoor || yCoor != prevyCoor)
    {
      setServo(xCoor, yCoor);
      prevxCoor = xCoor;
      prevyCoor = yCoor;
    }
    ball = 1;
  }
//  Serial.print(xServo);
//  Serial.print(yServo);
  sendBuffer[0] = 'X';
  sendBuffer[1] = xCoor;
  sendBuffer[2] = 'Y';
  sendBuffer[3] = yCoor;
  sendBuffer[4] = 'C';
  sendBuffer[5] = heading;
  sendBuffer[6] = 'B';
  sendBuffer[7] = ball;
  stmSerial.write(sendBuffer[0]);
  stmSerial.write(sendBuffer[1]);
  stmSerial.write(sendBuffer[2]);
  stmSerial.write(sendBuffer[3]);
  stmSerial.write(sendBuffer[4]);
  stmSerial.write(sendBuffer[5]);
  stmSerial.write(sendBuffer[6]);
  stmSerial.write(sendBuffer[7]);    
}
