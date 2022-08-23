// Pin data dari proximity masuk ke pin B3
#include "localization.h"
#include "speed_control.h"
#include "mekanisme_penendang.h"
#include <math.h>
#include "uart_node.h"
#include "uart_camera.h"
#include "uart_line.h"
#include "tm_stm32f4_delay.h"
#include "lcd_16x2.h"
#include "stdbool.h"
#include "stdio.h"
#include "tm_stm32f4_hcsr04.h"
#include "tm_stm32f4_gpio.h"
#include "stm32f4xx_exti.h"
#include "stm32f4xx_syscfg.h"
#include "misc.h"
#include "defines.h"

//Cepet
//#define VXMAKS 300
//#define VYJAUH 90
//#define VYDEKAT 70
//#define VYMAKS 90

//Normal
#define VXMAKS 100
#define VYJAUH 75
#define VYDEKAT 50
#define VYMAKS 75
//#define NYARIDELAY 30

//Pelan
//#define VXMAKS 50
//#define VYJAUH 45
//#define VYDEKAT 25
//#define VYMAKS 45


char lcd[16];

float KaP = 1;
float KaD = 2;
float KaI = 0;
float ball,errorBall,derivErrorBall,intErrorBall,prevErrorBall;
float ballMaju,errorBallMaju,derivErrorBallMaju,intErrorBallMaju,prevErrorBallMaju;
float heading;
float head,errorHead,derivErrorHead,intErrorHead,prevErrorHead;
float headGoal;
int headCount;
float heading, mov, movPrev, movX, movY;
int movCompass;
extern int compassHeading;
extern char gameState;
extern char ballXCoor;
extern char ballYCoor;
extern char ballX;
extern uint8_t ballFound;
extern float movY;
extern float movX;
extern int movCompass;
extern int x;
extern int y;
int counterDelay;
int rotateCount;
int icount=1;
int flagKickoff=0;
int prevCompass;
int sudutKanan = 315;
int sudutKiri = 45;
int flagmundur = 0;
int flagStrategy = 0;
int flagBackward = 0;
int rpmKanan;
int rpmKiri;
int handle = 0;
int strategyCount;
int keeperFlag = 0;
int keeperCount = 0;
int wasit = 1;
int tanda = 0;

extern int b;
/**
 * Start main merupakan fungsi utama program robot
 * fungsi ini akan mencari bola, dan gawang
 * @param refree untuk menentukan akan menggunakan refree atau tidak
 * @param mode untuk mengarahkan ke area musuh saja atau mengarahkan ke gawang musuh
 *
 */
void startMain(int refree, int arahGawang){
int prevGameState;
	if(refree == ON ){
		if (gameState == 's') {
			if (prevGameState == 'C' || prevGameState == 'F' || prevGameState == 'K' || prevGameState == 'N')
			{
				getBallNew();
				if(ballYCoor < 30)
				{
					kickBall();
					prevGameState = 's';
				}
			}
			else if (prevGameState == 'P')
			{
				getBallNew();
				if(ballYCoor < 33)
				{
					kickBall();
					prevGameState = 's';
				}
			}
			else if (prevGameState == 't' || prevGameState == 'c' || prevGameState == 'p' || prevGameState == 'f' || prevGameState == 'g' || prevGameState == 'k')
			{

				motorSpeed(0,0,0,0);
				for(counterDelay = 0; counterDelay < 20; counterDelay++)
				{
					Delayms(100);
					if(ballYCoor <= 90)
					{
						prevGameState = 's';
						break;
					}
				}
			}
			flagKickoff=0;
			if(ballYCoor>2){
				getBallNew();
				if(ballYCoor<=45){
					handleRotateIn();
					handle = 1;
				}
				else{
					handleOff();
				}
			}
			else{
				USART_SendData(USART2, 7);
				if(arahGawang == ON)
				{
					if(gotoHeadBack());
					{
//						kickBall();
						shootball();
					}
				}
				else
				{
					if(gotoHeadZero())
					{
//						kickBall();
						shootball();
					}
				}
			}
		}
		else if(gameState == 'T')
		{
			sudutKanan = 225;
			sudutKiri = 135;
			outBall1();
			prevGameState = 'T';
		}
		else if (gameState == 't')
		{
			sudutKanan = 135;
			sudutKiri = 225;
			outBall2();
			prevGameState = 't';
		}
		else if(gameState == 'C')
		{
			sudutKanan = 315;
			sudutKiri = 45;
			outBall1();
			prevGameState = 'C';
		}
		else if (gameState == 'c')
		{
			sudutKanan = 45;
			sudutKiri = 315;
			outBall2();
			prevGameState = 'c';
		}
		else if(gameState == 'P')
		{
			kickOff1();
			prevGameState = 'P';
		}
		else if (gameState == 'p')
		{
			kickOff2();
			prevGameState = 'p';
		}
		else if(gameState == 'F')
		{
			sudutKanan = 225;
			sudutKiri = 135;
			outBall1();
			prevGameState = 'F';
		}
		else if (gameState == 'f')
		{
			sudutKanan = 135;
			sudutKiri = 225;
			outBall2();
			prevGameState = 'f';
		}
		else if(gameState == 'G')
		{
			motorSpeed(0,0,0,0);
			for(counterDelay = 0; counterDelay < 20; counterDelay++)
			{
				Delayms(100);
				if(ballFound == 1)
				{
					prevGameState = 's';
					break;
				}
			}
			prevGameState = 's';
		}
		else if (gameState == 'g')
		{
			sudutKanan = 45;
			sudutKiri = 315;
			outBall2();
			prevGameState = 'g';
		}
		else if (gameState == 'K')
		{
			kickOff1();
			prevGameState = 'K';
		}
		else if (gameState == 'k')
		{
			sudutKanan = 45;
			sudutKiri = 315;
			kickOff2();
			prevGameState = 'k';
		}
		else if (gameState == 'N')
		{
			kickOff1();
			prevGameState = 'N';
		}
		else if(gameState == 'S')
		{
			handleOff();
			motorSpeed(0,0,0,0);
		}
		else
		{
			if(gameState == '$')
			{
				kiri(120);
			}
			else if(gameState == '%')
			{
				kanan(120);
			}
			else if(gameState == '@')
			{
				maju(120);
			}
			else if(gameState == '#')
			{
				mundur(120);
			}
			else if(gameState == '^')
			{
				rotateAntiClockWise(90);
			}
			else if(gameState == '&')
			{
				rotateClockWise(90);
			}
			else if(gameState == '*')
			{
				motorSpeed(0,0,0,0);
			}
		}

	}
	// refree off
	else{
		if(ballYCoor>0){
			getBallNew();
			if(ballYCoor<40){
				handleRotateIn();
			}
			else{
				handleOff();
			}
		}
		else{
//			if(arahGawang == ON){
//				getGoalPos();
//			}

//			else{
				handleOff();
				shootball();
//				if (gotoHeadZero())
//				{
////					kickBall();
//					shootball();
//				}
//			}
		}

	}
}

/*
 * outBall1 merupakan fungsi untuk mengambil posisi sebelum kita throw in
 * fungsi ini akan menggerakkan robot mendekati bola dengan jarak
 */
void outBall1()
{
	if(flagKickoff == 0){
		if(ballYCoor >= 60 && ballYCoor <= 65 && ballFound == 1){
			if(compassHeading >= 0 && compassHeading <= 180)
			{
				prevCompass = compassHeading;
				rotateAntiClockWise(PID(compassHeading,0,40));
				kanan(40);
				Delayms(1000);
				maju(40);
				Delayms(1000);
				flagKickoff = 1;
			}
			else if(compassHeading > 180 && compassHeading < 360)
			{
				prevCompass = compassHeading;
				rotateAntiClockWise(PID(compassHeading,0,40));
				kiri(40);
				Delayms(1000);
				maju(40);
				Delayms(1000);
				flagKickoff = 1;
			}
			else{
				flagKickoff = 1;
			}
		}
		else if(ballYCoor < 55 && ballFound == 1)
		{
			mundur(30);
		}
		else
		{
			getBallNew();
		}
	}
	else if (flagKickoff == 1) {
		if(ballYCoor > 55 && ballYCoor < 70 && ballFound == 1){
			if(compassHeading > 42 && compassHeading < 48 || compassHeading > 312 && compassHeading < 318)
			{
				int speedKiri = PID(ballXCoor,servoMid,30);
				if(speedKiri <= 10 && speedKiri > 0)
				{
					speedKiri = 10;
				}
				else if(speedKiri > -10 && speedKiri < 0)
				{
					speedKiri = -10;
				}
				kiri(speedKiri);
				if((ballXCoor-servoMid) > -2 && (ballXCoor - servoMid) < 2)
				{
					motorSpeed(0,0,0,0);
				}
			}
			else {
				if(prevCompass >= 0 && prevCompass <= 180)
				{
					int rotateSpeed = PID(compassHeading,sudutKanan,30);
					if(rotateSpeed <= 10 && rotateSpeed > 0)
					{
						rotateSpeed = 10;
					}
					else if(rotateSpeed > -10 && rotateSpeed < 0)
					{
						rotateSpeed = -10;
					}
					motorSpeed(rotateSpeed,0,0,rotateSpeed);
				}
				else if(prevCompass > 180 && prevCompass < 360)
				{
					int rotateSpeed = PID(compassHeading,sudutKiri,30);
					if(rotateSpeed <= 10 && rotateSpeed > 0)
					{
						rotateSpeed = 10;
					}
					else if(rotateSpeed > -10 && rotateSpeed < 0)
					{
						rotateSpeed = -10;
					}
					motorSpeed(rotateSpeed,0,0,rotateSpeed);
				}
			}
		}
		else if(ballYCoor <= 55 && ballFound == 1)
		{
			mundur(30);
		}
		else
		{
			getBallNew();
		}
	}
}

/*
 * outBall2 merupakan fungsi untuk mengambil posisi sebelum musuh throw in
 * fungsi ini akan menggerakkan robot mendekati bola dengan jarak 3 meter
 */
void outBall2()
{
	if(flagKickoff == 0){
		if(ballYCoor >= 60 && ballYCoor <= 65 && ballFound == 1){
			if(compassHeading >= 0 && compassHeading <= 90)
			{
				prevCompass = compassHeading;
				rotateAntiClockWise(PID(compassHeading,0,50));
				kanan(40);
				Delayms(1000);
				maju(40);
				Delayms(1000);
				flagKickoff = 1;
			}
			else if(compassHeading >= 270 && compassHeading < 360)
			{
				prevCompass = compassHeading;
				rotateAntiClockWise(PID(compassHeading,0,50));
				kiri(40);
				Delayms(1000);
				maju(40);
				Delayms(1000);
				flagKickoff = 1;
			}
			else{
				flagKickoff = 1;
			}
		}
		else if(ballYCoor < 55 && ballFound == 1)
		{
			mundur(40);
		}
		else
		{
			getBallNew();
		}
	}
	else if (flagKickoff == 1) {
		if(ballYCoor > 55 && ballYCoor < 70 && ballFound == 1){
			if(compassHeading > 42 && compassHeading < 48 || compassHeading > 312 && compassHeading < 318)
			{
				if (flagmundur == 0)
				{
					int speedKiri = PID(ballXCoor,servoMid,30);
					if(speedKiri <= 10 && speedKiri > 0)
					{
						speedKiri = 10;
					}
					else if(speedKiri > -10 && speedKiri < 0)
					{
						speedKiri = -10;
					}
					kiri(speedKiri);
					if((ballXCoor-servoMid) > -5 && (ballXCoor - servoMid) < 5)
					{
						flagmundur = 1;
					}
				}
				else if (flagmundur == 1)
				{
					mundur(40);
					if (ballYCoor > 110)
					{
						motorSpeed(0,0,0,0);
					}
				}
			}
			else {
				if(prevCompass >= 0 && prevCompass <= 180)
				{
					int rotateSpeed = PID(compassHeading,sudutKanan,30);
					if(rotateSpeed <= 10 && rotateSpeed > 0)
					{
						rotateSpeed = 10;
					}
					else if(rotateSpeed > -10 && rotateSpeed < 0)
					{
						rotateSpeed = -10;
					}
					motorSpeed(rotateSpeed,0,0,rotateSpeed);
				}
				else if(prevCompass > 180 && prevCompass < 360)
				{
					int rotateSpeed = PID(compassHeading,sudutKiri,30);
					if(rotateSpeed <= 10 && rotateSpeed > 0)
					{
						rotateSpeed = 10;
					}
					else if(rotateSpeed > -10 && rotateSpeed < 0)
					{
						rotateSpeed = -10;
					}
					motorSpeed(rotateSpeed,0,0,rotateSpeed);
				}
			}
		}
		else if(ballYCoor <= 55 && ballFound == 1)
		{
			mundur(40);
		}
		else
		{
			if (flagmundur == 0)
			{
				getBallNew();
			}
		}
	}
}

void init_adc()
{
	TM_ADC_Init(ADC1, 12);
	TM_ADC_Init(ADC1, 13);
}

void keeper(int v)
{
//	if(getProxy() == 1)
//	{
//		keeperFlag = 1;
//		keeperCount++;
//		Delayms(10);
//		if(keeperCount == 200){
//			keeperFlag = 0;
//		}
//	}
//	else if(getProxy() == 2)
//	{
//		keeperFlag = 2;
//		keeperCount++;
//		Delayms(10);
//		if(keeperCount == 200){
//			keeperFlag = 0;
//		}
//	}
//	else{}
//	if(keeperFlag == 1)
//	{
//		if(ballFound == 1)
//		{
//			if(ballXCoor <= 138){
//				keeperFlag = 0;
//			}
//			motorSpeed(0,0,0,0);
//
//		}
//		else
//		{
//			kiri(50);
//		}
//	}
//	else if(keeperFlag == 2)
//	{
//		if(ballFound == 1)
//		{
//			if(ballXCoor >= 146){
//				keeperFlag = 0;
//			}
//			motorSpeed(0,0,0,0);
//		}
//		else
//		{
//			kanan(50);
//		}
//	}
	if(keeperFlag == 0)
	{
		keeperCount = 0;
		if(ballXCoor <= 138 && ballFound == 1)
		{

			if(ballXCoor >= servoMid - 10 && ballXCoor <= servoMid + 10)
			{
				motorSpeed(0,0,0,0);
			}
			else{
				kiri(v);
			}
		}
		else if(ballXCoor >= 146 && ballFound == 1)
		{

			if(ballXCoor >= servoMid - 10 && ballXCoor <= servoMid + 10)
			{
				motorSpeed(0,0,0,0);
			}
			else{
				kanan(v);
			}
		}
		else
		{
			motorSpeed(0,0,0,0);
		}
//		if(ballFound == 0)
//		{
//			if(flagStrategy == 0)
//			{
//				if(compassHeading >= 320 && compassHeading < 360)
//				{
//					rotateClockWise(10);
//					if(compassHeading <= 320 && compassHeading > 300)
//					{
//						flagStrategy = 1;
//					}
//				}
//				else if(compassHeading < 300 && compassHeading >= 0)
//				{
//					rotateAntiClockWise(10);
//					if(compassHeading <= 320 && compassHeading > 300)
//					{
//						flagStrategy = 1;
//					}
//				}
//			}
//			if(flagStrategy == 1)
//			{
//				motorSpeed(0,0,0,0);
//			}
//		}
	}
}



/*
 * penalty merupakan fungsi untuk mengambil posisi sebelum penalty
 * fungsi ini akan menggerakkan robot mendekati bola
 */
void penalty(){
	if(ballYCoor > 55){
		getBallNew();
		//handleOn();
	}
	else{
			motorSpeed(0,0,0,0);
	}
}

void kickOff1(int v)
{
	if(flagKickoff == 0){
		if(ballYCoor >= 60 && ballYCoor <= 65 && ballFound == 1){
			if(compassHeading >= 315 && compassHeading < 360)
			{
				rotateAntiClockWise(PID(compassHeading,0,v));
				kiri(v);
				Delayms(3500);
				maju(v);
				Delayms(5000);
				flagKickoff = 1;
			}
			else if(compassHeading >= 270 && compassHeading < 315)
			{
				rotateAntiClockWise(PID(compassHeading,0,v));
				kiri(v);
				Delayms(2500);
				maju(v);
				Delayms(4000);
				flagKickoff = 1;
			}
			else if(compassHeading > 0 && compassHeading <= 45)
			{
				rotateClockWise(PID(compassHeading,0,v));
				kanan(v);
				Delayms(3500);
				maju(v);
				Delayms(5000);
				flagKickoff = 1;
			}
			else if(compassHeading > 45 && compassHeading <= 90)
			{
				rotateAntiClockWise(PID(compassHeading,0,v));
				kanan(v);
				Delayms(2500);
				maju(v);
				Delayms(4000);
				flagKickoff = 1;
			}
			else{
				flagKickoff = 1;
			}
		}
		else if(ballYCoor < 55 && ballFound == 1)
		{
			mundur(v-10);
		}
		else
		{
			getBallNew();
		}
	}
	else if (flagKickoff == 1) {
		if(ballYCoor > 55 && ballYCoor < 70 && ballFound == 1){
			if(compassHeading > 187 && compassHeading < 193)
			{
				int speedKiri = PID(ballXCoor,servoMid,v);
				if(speedKiri <= 10 && speedKiri > 0)
				{
					speedKiri = 10;
				}
				else if(speedKiri > -10 && speedKiri < 0)
				{
					speedKiri = -10;
				}
				kiri(speedKiri);
				if((ballXCoor-servoMid) > -5 && (ballXCoor - servoMid) < 5)
				{
					motorSpeed(0,0,0,0);
				}
			}
			else {
				int rotateSpeed = PID(compassHeading,190,v);
				if(rotateSpeed <= 10 && rotateSpeed > 0)
				{
					rotateSpeed = 10;
				}
				else if(rotateSpeed > -10 && rotateSpeed < 0)
				{
					rotateSpeed = -10;
				}
				motorSpeed(rotateSpeed,0,0,rotateSpeed);
			}
		}
		else if(ballYCoor <= 55 && ballFound == 1)
		{
			mundur(v);
		}
		else
		{
			getBallNew();
		}
	}
}

void kickOff2()
{
	if(flagKickoff == 0){
		if(ballYCoor >= 60 && ballYCoor <= 65 && ballFound == 1){
			if(compassHeading >= 315 && compassHeading < 360)
			{
				rotateAntiClockWise(PID(compassHeading,0,50));
				kiri(40);
				Delayms(3500);
				maju(40);
				Delayms(5000);
				flagKickoff = 1;
			}
			else if(compassHeading >= 270 && compassHeading < 315)
			{
				rotateAntiClockWise(PID(compassHeading,0,50));
				kiri(40);
				Delayms(2500);
				maju(40);
				Delayms(4000);
				flagKickoff = 1;
			}
			else if(compassHeading > 0 && compassHeading <= 45)
			{
				rotateClockWise(PID(compassHeading,0,50));
				kanan(40);
				Delayms(3500);
				maju(40);
				Delayms(5000);
				flagKickoff = 1;
			}
			else if(compassHeading > 45 && compassHeading <= 90)
			{
				rotateAntiClockWise(PID(compassHeading,0,50));
				kanan(40);
				Delayms(2500);
				maju(40);
				Delayms(4000);
				flagKickoff = 1;
			}
			else{
				flagKickoff = 1;
			}
		}
		else if(ballYCoor < 55 && ballFound == 1)
		{
			mundur(30);
		}
		else
		{
			getBallNew();
		}
	}
	else if (flagKickoff == 1) {
		if(ballYCoor > 55 && ballYCoor < 70 && ballFound == 1){
			if(compassHeading > 187 && compassHeading < 193)
			{
				if (flagmundur == 0)
				{
					int speedKiri = PID(ballXCoor,servoMid,30);
					if(speedKiri <= 10 && speedKiri > 0)
					{
						speedKiri = 10;
					}
					else if(speedKiri > -10 && speedKiri < 0)
					{
						speedKiri = -10;
					}
					kiri(speedKiri);
					if((ballXCoor-servoMid) > -5 && (ballXCoor - servoMid) < 5)
					{
						flagmundur = 1;
					}
				}
				else if (flagmundur == 1)
				{
					mundur(40);
					if (ballYCoor > 95)
					{
						motorSpeed(0,0,0,0);
					}
				}
			}
			else {
				int rotateSpeed = PID(compassHeading,190,30);
				if(rotateSpeed <= 10 && rotateSpeed > 0)
				{
					rotateSpeed = 10;
				}
				else if(rotateSpeed > -10 && rotateSpeed < 0)
				{
					rotateSpeed = -10;
				}
				motorSpeed(rotateSpeed,0,0,rotateSpeed);
			}
		}
		else if(ballYCoor <= 55 && ballFound == 1)
		{
			mundur(30);
		}
		else
		{
			if (flagmundur == 0)
			{
				getBallNew();
			}
		}
	}
}

void printData(){
	lcd_clear();
	int xCoor,yCoor;
	xCoor = movX;
	yCoor = movY;
	sprintf(lcd,"H:%d Y:%d X:%d ",compassHeading,ballYCoor,5);//ballXCoor);
	lcd_putsf(0,0,lcd);
	sprintf(lcd, "G:%c y:%d x:%d",'A',ballYCoor,ballXCoor);
	lcd_putsf(0,1,lcd);
}

/*
 *
 */
void init_proximity()
{
	GPIO_InitTypeDef gpio_init;
	EXTI_InitTypeDef EXTI_InitStruct;
	NVIC_InitTypeDef NVIC_InitStruct;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

	gpio_init.GPIO_Pin  = GPIO_PIN_13;
	gpio_init.GPIO_Mode = GPIO_Mode_IN;
	gpio_init.GPIO_Speed = GPIO_Speed_100MHz;
	gpio_init.GPIO_OType = GPIO_OType_PP;
	gpio_init.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_Init(GPIOC, &gpio_init);
//
//	/* Connect pin to interrupt */
//	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOC, EXTI_PinSource13);
//
//
//	/* Configure external interrupt */
//	EXTI_InitStruct.EXTI_Line = EXTI_Line13;
//	EXTI_InitStruct.EXTI_LineCmd = ENABLE;
//	EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
//	EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Rising;
//	EXTI_Init(&EXTI_InitStruct);
//
//	/* Add interrupt to NVIC */
//	NVIC_InitStruct.NVIC_IRQChannel = EXTI15_10_IRQn;
//	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0x03;
//	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0x00;
//	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
//	NVIC_Init(&NVIC_InitStruct);
//
	init_proximity2();
}

void init_proximity2()
{
	GPIO_InitTypeDef gpio_init;
	EXTI_InitTypeDef EXTI_InitStruct;
	NVIC_InitTypeDef NVIC_InitStruct;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

	gpio_init.GPIO_Pin  = GPIO_PIN_14;
	gpio_init.GPIO_Mode = GPIO_Mode_IN;
	gpio_init.GPIO_Speed = GPIO_Speed_100MHz;
	gpio_init.GPIO_OType = GPIO_OType_PP;
	gpio_init.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_Init(GPIOC, &gpio_init);

//	/* Connect pin to interrupt */
//	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOC, EXTI_PinSource14);
//
//
//	/* Configure external interrupt */
//	EXTI_InitStruct.EXTI_Line = EXTI_Line14;
//	EXTI_InitStruct.EXTI_LineCmd = ENABLE;
//	EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
//	EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Rising;
//	EXTI_Init(&EXTI_InitStruct);
//
//	/* Add interrupt to NVIC */
//	NVIC_InitStruct.NVIC_IRQChannel = EXTI15_10_IRQn;
//	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0x03;
//	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0x00;
//	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
//	NVIC_Init(&NVIC_InitStruct);
}

//#ifdef ACTIVATEPROXY1
//void EXTI15_10_IRQHandler(void) {
//	if (EXTI_GetITStatus(EXTI_Line13) != RESET) {
//		x = GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_13);
//		y = GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_14);
//		/* Clear interrupt bit */
//		EXTI_ClearITPendingBit(EXTI_Line1);
//	}
//}
//
//#endif


/*
 * getProxy merupakan fungsi untuk menghasilkan nilai 0 atau 1
 * dari inputan proximity
 */
int getProxy()
{
	if (x == 1)
	{
		return 1;
	}
	else if (y == 1)
	{
		return 2;
	}
	else if ((x == 0 || y == 0 ) && keeperFlag == 0)
	{
		return 0;
	}
	else if (keeperFlag == 0)
	{
		return 0;
	}
}

/*
 * ballGet merupakan fungsi untuk mengetahui apakah
 * robot telah mendapatkan bola atau belum dengan
 * menggunakan proximity
 * fungsi ini akan memutar hadle jika telah mendapatkan bola
 * dan mencari apabila belum
 */
void ballGet()
{
	if (getProxy() == 0)
	{
		handleRotateIn();
//		if(compassHeading>180)
//		{
//			rotateAntiClockWise(20);
//		}
//		else if (compassHeading<180)
//		{
//			rotateClockWise(20);
//		}
	}
	else
	{
		handleOff();
		getBall();
	}
}

void getBallNew()
{
	int kedepan;
//	if(ballYCoor < 20)
//	{
////		kickBall();
//	}
	if(ballYCoor < 40)
	{
		kedepan = VYDEKAT;
//		kedepan = VYJAUH;
//		kedepan = VYMAKS;
//		handleRotateIn();
	}
	else if(ballYCoor < 50)
	{
//		handleRotateIn();
		kedepan = VYJAUH;
//		kedepan = VYMAKS;
	}
	else{
		kedepan = VYMAKS;
	}
	int tengah = PID(ballXCoor, servoMid, VXMAKS);
	if(ballFound == 1){
		motorSpeed(tengah - kedepan, tengah - kedepan, tengah + kedepan, tengah + kedepan);
	}
	else
	{
		//rotateClockWise(50);
		if(flagStrategy == 0)
		{
			prevCompass = compassHeading + 20;
			flagStrategy = 1;
		}
		else if(flagStrategy == 1)
		{
			rotateClockWise(50);
			if(compassHeading >= prevCompass - 5 && compassHeading <= prevCompass + 5)
			{
				flagStrategy = 2;
			}
		}
		else if(flagStrategy == 2)
		{
			rotateClockWise(50);
			if(compassHeading >= 177 && compassHeading <= 183)
			{
				maju(50);
				for(strategyCount = 0; strategyCount < 30; strategyCount++)
				{
					Delayms(100);
					if(ballFound == 1)
					{
						break;
					}
				}
				flagStrategy = 0;
			}
		}
		else{}
	}
}

////		if (wasit == 0)
////		{
////			maju(VXMAKS);
////			for(strategyCount = 0; strategyCount < NYARIDELAY; strategyCount++)
////			{
////				Delayms(100);
////				if(ballFound == 1)
////				{
////					break;
////				}
////			}
////			wasit++;
////		}
//		if(flagStrategy == 0)
//		{
//			prevCompass = compassHeading + 20;
//			flagStrategy = 1;
//		}
//		if(flagStrategy == 1)
//		{
//			rotateClockWise(50);
//			if(compassHeading >= prevCompass - 5 && compassHeading <= prevCompass + 5)
//			{
//				flagStrategy = 2;
//			}
//		}
//		if(flagStrategy == 2)
//		{
//			rotateClockWise(50);
//			if(compassHeading >= 177 && compassHeading <= 183)
//			{
//				maju(50);
//				for(strategyCount = 0; strategyCount < 30; strategyCount++)
//				{
//					Delayms(100);
//					if(ballFound == 1)
//					{
//						break;
//					}
//				}
//				flagStrategy = 0;
//			}
//		}
//	}
//}


//{
//	int tengah = PID(ballXCoor, servoMid, 100);
//	int kedepan = 75;
//	rpmKiri = tengah-kedepan;
//	rpmKanan = kedepan+tengah;
//	if(ballFound == 1){
//		if(jarakDepan < 25){
//			if(rpmKanan > 30){
//				int offsetKanan = rpmKanan - 30;
//				int perbandinganKanan = rpmKiri/rpmKanan;
//				rpmKanan = 30;
//				rpmKiri = rpmKiri + offsetKanan * perbandinganKanan;
//			}
//			if(rpmKiri < -30){
//				int offsetKiri = rpmKiri + 30;
//				int perbandinganKiri = rpmKanan/rpmKiri;
//				rpmKiri = -30;
//				rpmKanan = rpmKanan - offsetKiri * perbandinganKiri;
//			}
//		motorSpeed(rpmKiri, rpmKiri, rpmKanan, rpmKanan);
//		if(ballYCoor < 30)
//		{
//			motorSpeed(0,0,0,0);
//		}
//		}
//	}
//	else
//	{
////		prevCompass = compassHeading;
//		rotateAntiClockWise(50);
////		if (compassHeading == prevCompass)
////		{
////			int cariBolaCount;
////			getBallNew();
////			if (ballFound == 0 && compassHeading == 0)
////			{
////				maju(50);
////				for (cariBolaCount = 0; cariBolaCount < 70; cariBolaCount++)
////				{
////					Delayms(100);
////					if (ballFound == 1)
////					{
////						break;
////					}
////				}
////			}
////		}
//	}
//}

void getBall(void){
	if(ballFound == 1){
		if((ballXCoor-servoMid) > -15 && (ballXCoor - servoMid) < 15){
			errorBallMaju =  ballYCoor ;
			derivErrorBallMaju = errorBallMaju - prevErrorBallMaju;
			intErrorBallMaju += errorBallMaju;
			ballMaju = (float)KaP * errorBallMaju + (float)KaD * derivErrorBallMaju + (float)KaI * intErrorBallMaju;
			if(ballMaju>75){
				ballMaju = 75;
			}
			else if(ballMaju<20){
				ballMaju = 20;
			}
			if(jarakDepan < 70)
			{
				maju(PID(jarakDepan,40));
			}
			maju(ballMaju);
			prevErrorBallMaju=errorBallMaju;
		}
		else{
			errorBall = servoMid - ballXCoor ;
			derivErrorBall = errorBall - prevErrorBall;
			intErrorBall += errorBall;
			ball = (float)KaP * errorBall + (float)KaD * derivErrorBall + (float)KaI * intErrorBall;
			if(ball>50){
				ball = 50;
			}
			else if(ball<15 && ball >0){
				ball = 10;
			}
			else if(ball>-15 && ball<0){
				ball = -10;
			}
			else if (ball <-50){
				ball = -50;
			}
			rotateAntiClockWise(ball);
			prevErrorBall=errorBall;
		}
	}
	else{
		rotateAntiClockWise(20);
	}


}

int PID(int param, int desire, int vMaks){
	int temp;
	errorBall = desire - param ;
	derivErrorBall = errorBall - prevErrorBall;
	intErrorBall += errorBall;
	temp = (float)KaP * errorBall + (float)KaD * derivErrorBall + (float)KaI * intErrorBall;
	if(temp>vMaks){
		temp = vMaks;
	}
	else if(temp<10 && temp >0){
		temp = 5;
	}
	else if(temp>-10 && temp<0){
		temp = -5;
	}
	else if (temp <-vMaks){
		temp = -vMaks;
	}
	prevErrorBall=errorBall;
	return temp;
}

void getGoalPos(void){
	float x,y;
	x = goalX-movX;
	y = goalY-movY;
	headGoal = atan(y/x)*180/PI;
	 if (x>0 && y>0){
	   //kuadran1
	   headGoal = headGoal;
	   }

	 else if (x>0 && y<0){
	   //kuadran2
	   headGoal=headGoal+180;
	   }

	 else if (x<0 && y<0){
	   //kuadran3
	   headGoal=headGoal+180;
	   }

	 else {
	   //kuadran4
	   headGoal	=headGoal+360;
	   }

	 if((compassHeading-headGoal) > -10 && (compassHeading - headGoal) < 10){
		 handleOff();
		 maju(50);
	 }
	 else{
		 if(headGoal>180){
			 rotateClockWise(20);
		 }
		 else if (headGoal<180) {
			rotateAntiClockWise(20);
		}
	 }

	 prevErrorHead=errorHead;

}

bool gotoHeadZero(void){
	 if(compassHeading > 170 && compassHeading < 190){
//	if(compassHeading > 2 && compassHeading < 5){
		 //handleRotateOut();
		 //maju(50);
		motorSpeed(0,0,0,0);
		 return true;
	 }
	 else{
		rotateClockWise(70);
		handleRotateIn();
//		if(PID(compassHeading,180))
//		{
//			rightHandleRotateIn();
//		}
//		else if(!PID(compassHeading,180))
//		{
//			leftHandleRotateIn();
//		}
		 return false;
	 }
}

int gotoHeadBack(void){
	 if(compassHeading > 358 && compassHeading < 3){
		if(flagBackward == 0)
		{
			if(ballX > 127)
			{
				kanan(50);
			}
			else if(ballX < 123)
			{
				kiri(50);
			}
			else
			{
				flagBackward = 1;
			}
		}
		else if(flagBackward == 1)
		{
			if(ballFound == 1)
			{
				mundur(50);
			}
			else
			{
				if(gotoHeadZero())
				{
					motorSpeed(0,0,0,0);
					flagBackward = 0;
					return 1;
				}
			}
		}
	 }
	 else{
		rotateClockWise(70);
		handleRotateIn();
//		if(PID(compassHeading,180))
//		{
//			rightHandleRotateIn();
//		}
//		else if(!PID(compassHeading,180))
//		{
//			leftHandleRotateIn();
//		}
		 return 0;
	 }
}

void getCoor(int headingProcess,int encoderDifference){
    	if(headingProcess == 1){
//    		if(headCount > 586) headCount -= 586;
//    		if(headCount < 0) headCount += 586;
//        	heading = (float)headCount*360/586;
    	}
    	else{
//    		mov = (float) encoderDifference/(14);
//    		if(mov - movPrev > 0.0007 || mov - movPrev < - 0.0007){
    		mov = (float) encoderDifference;
//    		if(mov - movPrev > -1 && mov - movPrev < 1){
//    			movX = movX + mov * cos (compassHeading*PI/180);
//    			movY = movY + mov * sin (compassHeading*PI/180);
    			movX = movX + mov * cos (compassHeading);
				movY = movY + mov * sin (compassHeading);
    		}
    		movPrev = mov;
//    	}
}

void gotoXY(int ngadep, int coorX, int coorY)
{
	headGoal = atan(coorY/coorX);
	if (tanda == 0)
	{
		if (compassHeading >= headGoal - 1 && compassHeading <= headGoal + 1)
		{
			if ((int)movX == coorX && (int)movY == coorY)
			{
				tanda = 1;
			}
			else
			{
				maju(PID(movX, coorX, 50));
			}
		}
		else
		{
			if(compassHeading<180){
				rotateAntiClockWise(PID(compassHeading,headGoal,30));
			}
			else if (compassHeading>180) {
				rotateClockWise(PID(compassHeading,headGoal,30));
			}
		}
	}
	else if(tanda == 1)
	{
		if(compassHeading<180){
			rotateAntiClockWise(PID(compassHeading,ngadep,30));
		}
		else if (compassHeading>180) {
			rotateClockWise(PID(compassHeading,ngadep,30));
		}
		if(compassHeading >= ngadep - 4 && compassHeading <= ngadep + 4)
		{
			tanda = 2;
		}
	}
	else
	{
		motorSpeed(0,0,0,0);
	}
}

void tesRotateMap(void){
	int icount=1;
	while(compassHeading > (90 * icount)+2){
		rotateAntiClockWise(PID(compassHeading,(90 * icount)+2,50));
	}
	while(1){
		if(icount == 1){
				while(compassHeading > (90 * icount)+2){
					rotateAntiClockWise(PID(compassHeading,(90 * icount)+2,50));
				}
		}

		while(compassHeading < (90 * icount)){
			rotateAntiClockWise(PID(compassHeading,(90 * icount),50 ));
		}
		maju(20);
		Delayms(2000);
		rotateAntiClockWise(0);
		Delayms(2000);
		icount+=2;
		if (icount > 3){
			icount = 1;
		}
	}
}


void LCD_InitializeTimer()
{
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);

	// TIM clock = 84 MHz,
	//Update event time = 1 / [(84 * 10^6) / (TIM_Prescaler * TIM_Period)]
    //kalo mau sedetik: pre = 42000-1, per = 2000-1
    TIM_TimeBaseInitTypeDef timerInitStructure;

    timerInitStructure.TIM_Prescaler = 42000-1;
    timerInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
    timerInitStructure.TIM_Period = 1000-1;
    timerInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    timerInitStructure.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(TIM5, &timerInitStructure);
    TIM_Cmd(TIM5, ENABLE);
	TIM_ITConfig(TIM5, TIM_IT_Update, ENABLE);

}
void LCD_EnableTimerInterrupt()
{
    NVIC_InitTypeDef nvicStructure;

    nvicStructure.NVIC_IRQChannel = TIM5_IRQn;
    nvicStructure.NVIC_IRQChannelPreemptionPriority = 0;
    nvicStructure.NVIC_IRQChannelSubPriority = 0;
    nvicStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&nvicStructure);

}

void TIM5_IRQHandler(){
	if (TIM_GetITStatus(TIM5, TIM_IT_Update) != RESET)
	    {
	        TIM_ClearITPendingBit(TIM5, TIM_IT_Update);
	        if(b == 0)
	        {
	        	printMenu();
	        }
	        else
	        {
	        	printData();
	        }
	    }
}

void init_lcdTimer(){
	LCD_InitializeTimer();
	LCD_EnableTimerInterrupt();
}
