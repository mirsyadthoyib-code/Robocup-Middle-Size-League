#include <stm32f4xx_gpio.h>
#include <stm32f4xx_tim.h>
#include <stm32f4xx_rcc.h>
#include <misc.h>
#include "interrupt_tim.h"
#include "tm_stm32f4_rotary_encoder.h"
#include "stdint.h"
#include "stdio.h"
#include "pwm_motor.h"
#include "lcd_16x2.h"
#include <math.h>

#define KP 10.8 //2.8
#define KD 280 //1.6
#define KI 0 //0.1


//char lcd[16];
TM_RE_t data;
TM_RE_t data2;

void InitializeTimer()
{
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);

    //kalo mau sedetik: pre = 42000-1, per = 2000-1
    TIM_TimeBaseInitTypeDef timerInitStructure;
    TIM_TimeBaseInitTypeDef timInitDua;

    timerInitStructure.TIM_Prescaler = 42000-1;
    timerInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
    timerInitStructure.TIM_Period = 10-1;
    timerInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    timerInitStructure.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(TIM2, &timerInitStructure);
    TIM_Cmd(TIM2, ENABLE);
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);

	timInitDua.TIM_Prescaler = 42000-1;
	timInitDua.TIM_CounterMode = TIM_CounterMode_Up;
	timInitDua.TIM_Period = 10-1;
	timInitDua.TIM_ClockDivision = TIM_CKD_DIV1;
	timInitDua.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(TIM5, &timInitDua);
    TIM_Cmd(TIM5, ENABLE);
	TIM_ITConfig(TIM5, TIM_IT_Update, ENABLE);
}

void EnableTimerInterrupt()
{
    NVIC_InitTypeDef nvicStructure;
    NVIC_InitTypeDef nvicStructDua;

    nvicStructure.NVIC_IRQChannel = TIM2_IRQn;
    nvicStructure.NVIC_IRQChannelPreemptionPriority = 0;
    nvicStructure.NVIC_IRQChannelSubPriority = 1;
    nvicStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&nvicStructure);

    nvicStructDua.NVIC_IRQChannel = TIM5_IRQn;
    nvicStructDua.NVIC_IRQChannelPreemptionPriority = 0;
    nvicStructDua.NVIC_IRQChannelSubPriority = 1;
    nvicStructDua.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&nvicStructDua);
}

void TIM2_IRQHandler()
{
    if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
    {
        TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
        TM_RE_Get(&data);

        rpm = (int32_t)(data.Diff*60*2/8);

        error = setPointSatu - rpm;
        derivError = error - prevError;
        intError += error;
        output += (float)KP * error + (float)KD * derivError + (float)KI * intError;

        if (output > 65534) output = 65534;
        motorDC(1, output);

        prevError = error;
    }
}

void TIM5_IRQHandler()
{
    if (TIM_GetITStatus(TIM5, TIM_IT_Update) != RESET)
    {
        TIM_ClearITPendingBit(TIM5, TIM_IT_Update);
        SecondGet(&data2);

        rpmDua = (int32_t)(data2.Diff*2*60/8);

        errorDua = setPointDua - rpmDua;
        derivErrorDua = errorDua - prevErrorDua;
        intError += error;
        outputDua += (float)KP * errorDua + (float)KD * derivErrorDua + (float)KI * intErrorDua;

        if (outputDua > 65534) outputDua = 65534;
        motorDC(2, outputDua);

        prevErrorDua = errorDua;
    }
}

void motorSpeed(int16_t leftSpeed, int rightSpeed) {
	//these two variables are interchangeably, depending on robots' need
	setPointSatu = leftSpeed;
	setPointDua = rightSpeed;
}

void init_tim(void)
{
	InitializeTimer();
	EnableTimerInterrupt();
	TM_RE_Init(&data);
	InitSecondEncoder(&data2);
}
