#include "stm32f4xx_exti.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_syscfg.h"
#include "misc.h"
#include "tm_stm32f4_rotary_encoder.h"
#include "tm_stm32f4_delay.h"
#include "tm_stm32f4_hcsr04.h"
#include "tm_stm32f4_gpio.h"
#include "tm_stm32f4_adc.h"
#include "lcd_16x2.h"
#include "pwm_motor.h"
#include "stdio.h"
#include "speed_control.h"
#include <math.h>
#include "uart_node.h"
#include "uart_camera.h"
#include "uart_line.h"
#include "localization.h"
#include "mekanisme_penendang.h"

int counterSrf = 0;
uint8_t adcdatakiri;
uint8_t adcdatakanan;
uint8_t received_val = 0;

void tesKicker(){
	kick(ON);
	Delayms(1000);
	kick(OFF);
	Delayms(5000);

}


void tesHandle(){
	handleRotateIn();
	Delayms(2000);
	handleRotateOut();
	Delayms(2000);
	leftHandleRotateIn();
	Delayms(2000);
	rightHandleRotateIn();
	Delayms(2000);
	handleOff();
	Delayms(2000);
}

void tesMenghindar()
{
	if(jarakDepan < 40){
		kanan(50);
		Delayms(1000);
		kanan(0);
	}
}

void tesMotor(int rpm){
	maju(rpm);
	Delayms(2000);
	mundur(rpm);
	Delayms(2000);
	kiri(rpm);
	Delayms(2000);
	kanan(rpm);
	Delayms(2000);
	baratLaut(rpm);
	Delayms(2000);
	tenggara(rpm);
	Delayms(2000);
	baratDaya(rpm);
	Delayms(2000);
	timurLaut(rpm);
	Delayms(2000);
	rotateAntiClockWise(rpm);
	Delayms(2000);
	rotateClockWise(rpm);
	Delayms(2000);
}

void tesMapping()
{
	maju(50);
	Delayms(2000);
	rotateClockWise(50);

}

int x, y;
int main(void)
{
	SystemInit();

	lcd_init();
	TM_DELAY_Init();
	init_lcdTimer();
	lcd_clear();


//	init_proximity();
	init_speed_control();
	init_handle();
	init_node();
	init_camera();
//	init_line();
	init_penendang();
	init_menu();
//	init_srf();
//	init_adc();
	handleRotateIn();
	Delayms(1000);
	handleOff();
//	GPIO_SetBits(GPIOC,GPIO_Pin_9);
//	GPIO_SetBits(GPIOA,GPIO_Pin_7);
//	GPIO_SetBits(GPIOA,GPIO_Pin_6);

//	maju(240);
//	Delayms(1800);
    while(1)
    {
//    	menu();
//    	x = GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_13);
//    	y = GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_14);
//    	kickOff1();
//    	tesRotateMap();
//    	tesMapping();
//    	getProxy();
//    	ballGet();
//    	getBall();
//    	tesKicker();
//    	tesHandle();
//    	tesMotor(30);
//    	maju(30);
//    	mundur(30);
//    	shootball();
//    	maju(600);
//    	motorSpeed(0,-600,0,600);
//    	Delayms(500);
//    	motorSpeed(0,0,0,0);
//    	penendang(50);
//    	Delayms(2000);
//    	counterSrf++;
//    	if(counterSrf == 1000){
//    		tesSRF();
//    		counterSrf =0;
//    	}
//    	tesSRF();
//    	keeper();
//    	startMain(ON,ON);
//    	startMain(ON,OFF);
//    	startMain(OFF,OFF);
//    	shootball();
//    	getBallNew();
//    	kickBall();
//    	keeper(100);
//    	getBallNew();
//    	motorSpeed(1000,0,0,0);
//    	kickBall();
//    	gotoXY(100,50,0);
    	handleRotateIn();
//    	outBall1();
//    	outBall2();
//    	Delayms(100);
//    	penalty();
//    	maju(30);
    }
}
