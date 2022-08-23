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
int tiltLoop = 75;
int panLoop = 125;
int counterBallDetection = 0;
int panError, tiltError;
int k = 125, l = 125;
int prevxCoor = 0, prevyCoor = 0;
int xservo = 125, yservo = 75;

Servo ServoY;
Servo ServoX;
SoftwareSerial stmSerial(5, 6);

void setServo(int kamerax, int kameray)
{
  while(kamerax != 125 || kameray != 125)
  {
    Serial.print(kamerax);
    Serial.println(kameray);
    Serial.println();
    if(kamerax != 125)
    {
      xservo = k;
      xservo = map(xservo, 0, 250, 0, 166);
      ServoX.write(xservo);
      if(kamerax > 125)
      {
        k--;
      }
      else if(kamerax < 125)
      {
        k++;
      }
      delay(5);
    }
    if(kameray != 125)
    {
      yservo = l;
      yservo = map(yservo, 0, 250, 0, 166);
      ServoY.write(yservo);
      if(kameray < 125)
      {
        l--;
      }
      else if(kameray > 125)
      {
        l++;
      }
      delay(5);
    }
  }
}

void directServo(int a, int b)
{
  a = map(a, 0, 250, 0, 166);
  b = map(b, 0, 250, 0, 166);
  ServoX.write(a);
  ServoY.write(b);
}

void setup() {
  Serial.begin(9600);
  stmSerial.begin(9600);
//  ServoY.attach(servoPinY);
//  ServoX.attach(servoPinX);
//  directServo(xservo, yservo);
}

void loop() 
{
  if(Serial.available())
  {
    data = Serial.read();
  }
  else{}
  if(flag=='X')
  {
    xCoor = data;
    flag = 0;
  }
  else if(flag=='Y')
  {
    yCoor = data;
    flag = 0;
  }
  else
  {
    flag = data;
  }

//  if(xCoor == 0 && yCoor == 0)
//  {
//    counterBallDetection++;
//    if(counterBallDetection == 500)
//    {
//      xservo = 125;
//      yservo = 75;
////      setServo(xservo, yservo);
//      directServo(xservo, yservo);
//      counterBallDetection = 0;
//      ball = 0;
//    }
//  }
//  else
//  { 
//    Serial.print("sini");
//    setServo(xCoor, yCoor);
//    ball = 1;
//  }
  
//  sendBuffer[0] = 'X';
//  sendBuffer[1] = xservo;
//  sendBuffer[2] = 'Y';
//  sendBuffer[3] = yservo;
//  sendBuffer[4] = 'C';
//  sendBuffer[5] = heading;
//  sendBuffer[6] = 'B';
//  sendBuffer[7] = ball;
//  stmSerial.write(sendBuffer[0]);
//  stmSerial.write(sendBuffer[1]);
//  stmSerial.write(sendBuffer[2]);
//  stmSerial.write(sendBuffer[3]);
//  stmSerial.write(sendBuffer[4]);
//  stmSerial.write(sendBuffer[5]);
//  stmSerial.write(sendBuffer[6]);
//  stmSerial.write(sendBuffer[7]);    
}
