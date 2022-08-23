// speed_control.c & speed_control.h: Adika Bintang Sulaeman, Jan 16th 2015
// edited Jan 11th 2017 by Yunus Maulana
// update notes : 4 motor with 4 encoder
// This code was tested using Pololu motor 19:1 with encoder with max speed: 200 rpm
// To use this code, one should plug the following cables to the microcrontroller:
// motor 1   : in2: PB12, in1: PB13
// encoder 1 : Yellow PD0, White PD4
// motor 2   : in3: PB14, in4: PB15
// encoder 2 : Yellow PD1, White PD5
// motor 3   : in3: PE12, in4: PE14
// encoder 3 : Yellow PD2, White PD6
// motor 4   : in3: PE13, in4: PE15
// encoder 4 : Yellow PD3, White PD7
// dependencies: as they are included in #include
// Before implementing code in while (1), you have to call void init_speed_control(void) inside int main()
// To set the motor speed, use motorSpeed(kecepatan kiri, kecepatan kanan);
// Credit to: Tilen Majerle (stm32f4-discovery.com)

// Important:
// after initialize, directly call motorSpeed(0,0);
// if you want motor to stop, always motorSpeed(0, 0);
// e.g.: muter: motorSpeed(0,0); motorSpeed(-60,60); Delayms(1000); motorSpeed(0,0);

#include <stm32f4xx_gpio.h>
#include <stm32f4xx_tim.h>
#include <stm32f4xx_rcc.h>
#include <misc.h>
#include "speed_control.h"
#include "tm_stm32f4_rotary_encoder.h"
#include "pwm_motor.h"
#include "stdint.h"
#include <math.h>
#include "uart_node.h"
#include "localization.h"
#include "tm_stm32f4_hcsr04.h"
#include "tm_stm32f4_gpio.h"

#define PI 3.14159265
#define KP 5
#define KD 10
#define KI 0

int8_t startHeading;
int8_t prevStartHeading;
int32_t rotaryCount = 0;
int32_t lastCount = 0;
int32_t speed = 0;
int rata2;
int penyerang = 1;

//motor1
int32_t output = 0;
int16_t setPointSatu = 0;
int32_t rpm = 0;
int16_t error = 0;
int16_t prevError = 0;
int16_t derivError = 0;
int16_t intError = 0;

//motor2
int32_t outputDua = 0;
int16_t setPointDua = 0;
int32_t rpmDua = 0;
int16_t errorDua = 0;
int16_t prevErrorDua = 0;
int16_t derivErrorDua = 0;
int16_t intErrorDua = 0;

//motor3
int32_t outputTiga = 0;
int16_t setPointTiga = 0;
int32_t rpmTiga = 0;
int16_t errorTiga = 0;
int16_t prevErrorTiga = 0;
int16_t derivErrorTiga = 0;
int16_t intErrorTiga = 0;

//motor4
int32_t outputEmpat = 0;
int16_t setPointEmpat = 0;
int32_t rpmEmpat = 0;
int16_t errorEmpat = 0;
int16_t prevErrorEmpat = 0;
int16_t derivErrorEmpat = 0;
int16_t intErrorEmpat = 0;

//motorKicker
int32_t outputKicker = 0;
int16_t setPointKicker = 0;
int32_t rpmKicker = 0;
int16_t errorKicker = 0;
int16_t prevErrorKicker = 0;
int16_t derivErrorKicker = 0;
int16_t intErrorKicker = 0;

TM_RE_t data;
TM_RE_t data2;
TM_RE_t data3;
TM_RE_t data4;
TM_RE_t data5;
TM_RE_t data6;
TM_RE_t dataKicker;

void changeStartHeading(int now) {
	startHeading = now;
}


void InitializeTimer()
{
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

	// TIM clock = 84 MHz,
	//Update event time = 1 / [(84 * 10^6) / (TIM_Prescaler * TIM_Period)]
    //kalo mau sedetik: pre = 42000-1, per = 2000-1
    TIM_TimeBaseInitTypeDef timerInitStructure;

    timerInitStructure.TIM_Prescaler = 39300-1;
    timerInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
    timerInitStructure.TIM_Period = 10-1;
    timerInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    timerInitStructure.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(TIM2, &timerInitStructure);
    TIM_Cmd(TIM2, ENABLE);
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);

}

void EnableTimerInterrupt()
{
    NVIC_InitTypeDef nvicStructure;

    nvicStructure.NVIC_IRQChannel = TIM2_IRQn;
    nvicStructure.NVIC_IRQChannelPreemptionPriority = 0;
    nvicStructure.NVIC_IRQChannelSubPriority = 1;
    nvicStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&nvicStructure);

}

void TIM2_IRQHandler()
{

    if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
    {

        TIM_ClearITPendingBit(TIM2, TIM_IT_Update);

        //update motor 1 (left rear)
        TM_RE_Get(&data);

        rpm = (int32_t)(data.Diff*60*1.37);
        error = setPointSatu - rpm;
        derivError = error - prevError;
        intError += error;
        output += (float)KP * error + (float)KD * derivError + (float)KI * intError;

        if (output > 65534) output = 65534;
        motorDC(1, output);

        prevError = error;

        //update motor 2 (left front)
        SecondGet(&data2);

        rpmDua = (int32_t)(data2.Diff*60*1.37);

        errorDua = setPointDua - rpmDua;
        derivErrorDua = errorDua - prevErrorDua;
        intErrorDua += errorDua;
        outputDua += (float)KP * errorDua + (float)KD * derivErrorDua + (float)KI * intErrorDua;

        if (outputDua > 65534) outputDua = 65534;
        motorDC(2, outputDua);

        prevErrorDua = errorDua;

        //update motor 3 (right front)
        ThirdGet(&data3);

        rpmTiga = (int32_t)(data3.Diff*60*1.37);

        errorTiga = setPointTiga - rpmTiga;
        derivErrorTiga = errorTiga - prevErrorTiga;
        intErrorTiga += errorTiga;
        outputTiga += (float)KP * errorTiga + (float)KD * derivErrorTiga + (float)KI * intErrorTiga;

        if (outputTiga > 65534) outputTiga = 65534;
        motorDC(3, outputTiga);

        prevErrorTiga = errorTiga;

        //update motor 4 (right rear)
        FourthGet(&data4);

        rpmEmpat = (int32_t)(data4.Diff*60*1.37);

        errorEmpat = setPointEmpat - rpmEmpat;
        derivErrorEmpat = errorEmpat - prevErrorEmpat;
        intErrorEmpat += errorEmpat;
        outputEmpat += (float)KP * errorEmpat + (float)KD * derivErrorEmpat + (float)KI * intErrorEmpat;

        if (outputEmpat > 65534) outputEmpat = 65534;
        motorDC(4, outputEmpat);

        prevErrorEmpat = errorEmpat;

        //update motor kicker (right rear)
        KickerGet(&dataKicker);

		rpmKicker = (int32_t)(dataKicker.Diff*60*1.37);

		errorKicker = setPointKicker - rpmKicker;
		derivErrorKicker = errorKicker - prevErrorKicker;
		intErrorKicker += errorKicker;
		outputKicker += (float)KP * errorKicker + (float)KD * derivErrorKicker + (float)KI * intErrorKicker;

		if (outputKicker > 65534) outputKicker = 65534;
		penendang(outputKicker);

		prevErrorKicker = errorKicker;

//		FifthGet(&data5);
//		SixthGet(&data6);
//		rata2 = (data6.Diff - data5.Diff)/2;
//		getCoor(startHeading, rata2);

//		rata2 = (data4.Diff);
		getCoor(startHeading, data4.Diff);
    }
}

void motorSpeed(int16_t leftBackSpeed, int16_t leftFrontSpeed, int16_t righFrontSpeed, int rightBackSpeed) {
	//these two variables are interchangeably, depending on robots' need
//	if(penyerang==1)
//	{
//		setPointKicker = leftBackSpeed;
//	}
	setPointSatu = leftBackSpeed;
	setPointDua = leftFrontSpeed;
	setPointTiga = righFrontSpeed;
	setPointEmpat = rightBackSpeed;
}

void kickerSpeed(int16_t kicker)
{
	setPointKicker = kicker;
}

void init_speed_control(void)
{
	InitializeTimer();
	EnableTimerInterrupt();
	TM_RE_Init(&data);
	InitSecondEncoder(&data2);
	InitThirdEncoder(&data3);
	InitFourthEncoder(&data4);
//	InitFifthEncoder(&data5);
//	InitSixthEncoder(&data6);
	InitKickerEncoder(&dataKicker);


	init_pwm();
	init_pwm_penendang();
}

/**
  *semua roda maju
  *
  **/
void maju(int32_t rpm) {
//	motorDc(kaDepan, 1);
//	motorDc(kiDepan, 1);
//	motorDc(kaBelakang, 1);
//	motorDc(kiBelakang, 1);
	motorSpeed(-rpm,-rpm,rpm,rpm);
	startHeading = 0;
}

/**
  *semua roda mundur
  **/
void mundur(int32_t rpm) {
//	motorDc(kaDepan, -1);
//	motorDc(kiDepan, -1);
//	motorDc(kaBelakang, -1);
//	motorDc(kiBelakang, -1);
//	if (jarakBelakang < 25)
//	{
//		rpm = -PID(jarakBelakang,20,40);
//	}
	motorSpeed(rpm,rpm,-rpm,-rpm);
	changeStartHeading(0);
}

/**
  *roda kiri depan dan kanan belakang maju
  *vice versa
  **/
void kanan(int32_t rpm) {
//	motorDc(kaDepan, -1);
//	motorDc(kiDepan, 1);
//	motorDc(kaBelakang, 1);
//	motorDc(kiBelakang, -1);
	motorSpeed(rpm,-rpm,-rpm,rpm);
	changeStartHeading(0);
}

/**
  *roda kiri depan dan kanan belakang mundur
  *vice versa
  **/
void kiri(int32_t rpm) {
//	motorDc(kaDepan, 1);
//	motorDc(kiDepan, -1);
//	motorDc(kaBelakang, -1);
//	motorDc(kiBelakang, 1);
	motorSpeed(-rpm,rpm,rpm,-rpm);
	changeStartHeading(0);
}

/**
  *Ke kanan atas
  *kiri depan dan kanan belakang maju, sisanya diam
  **/
void timurLaut(int32_t rpm) {
//	motorDc(kaDepan, 0);
//	motorDc(kiDepan, 1);
//	motorDc(kaBelakang, 1);
//	motorDc(kiBelakang, 0);
	motorSpeed(0,-rpm,0,rpm);
	changeStartHeading(0);
}

/**
  *Ke kanan bawah
  *kiri depan dan kanan belakang mundur, sisanya diam
  **/
void tenggara(int32_t rpm) {
//	motorDc(kaDepan, 0);
//	motorDc(kiDepan, -1);
//	motorDc(kaBelakang, -1);
//	motorDc(kiBelakang, 0);
	motorSpeed(0,rpm,0,-rpm);
	changeStartHeading(0);
}

/**
  *Ke kiri atas
  *Kanan depan dan kiri belakang maju, sisanya diam
  **/
void baratLaut(int32_t rpm) {
//	motorDc(kaDepan, 1);
//	motorDc(kiDepan, 0);
//	motorDc(kaBelakang, 0);
//	motorDc(kiBelakang, 1);
	motorSpeed(-rpm,0,rpm,0);
	changeStartHeading(0);
}

/**
  *Ke kiri bawaj
  *Kanan depan dan kiri belakang mundur, sisanya diam
  **/
void baratDaya(int32_t rpm) {
//	motorDc(kaDepan, -1);
//	motorDc(kiDepan, 0);
//	motorDc(kaBelakang, 0);
//	motorDc(kiBelakang, -1);
	motorSpeed(rpm,0,-rpm,0);
	changeStartHeading(0);
}

/**
  * roda kanan depan dan kanan belakang arahnya ke belakang
  * vice versa
  **/
void rotateClockWise(int32_t rpm) {
//	motorDc(kaDepan, -1);
//	motorDc(kiDepan, 1);
//	motorDc(kaBelakang, -1);
//	motorDc(kiBelakang, 1);
	motorSpeed(-rpm,-rpm,-rpm,-rpm);
	changeStartHeading(1);
}


/**
  * roda kanan depan dan kanan belakang arahnya ke depan
  * vice versa
  **/
void rotateAntiClockWise(int32_t rpm) {
//	motorDc(kaDepan, 1);
//	motorDc(kiDepan, -1);
//	motorDc(kaBelakang, 1);
//	motorDc(kiBelakang, -1);
	motorSpeed(rpm,rpm,rpm,rpm);
	changeStartHeading(1);
}


