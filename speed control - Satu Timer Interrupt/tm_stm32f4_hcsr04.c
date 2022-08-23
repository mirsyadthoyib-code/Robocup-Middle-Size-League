/**	
 * |----------------------------------------------------------------------
 * | Copyright (C) Tilen Majerle, 2014
 * | 
 * | This program is free software: you can redistribute it and/or modify
 * | it under the terms of the GNU General Public License as published by
 * | the Free Software Foundation, either version 3 of the License, or
 * | any later version.
 * |  
 * | This program is distributed in the hope that it will be useful,
 * | but WITHOUT ANY WARRANTY; without even the implied warranty of
 * | MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * | GNU General Public License for more details.
 * | 
 * | You should have received a copy of the GNU General Public License
 * | along with this program.  If not, see <http://www.gnu.org/licenses/>.
 * |----------------------------------------------------------------------
 */
#include "tm_stm32f4_hcsr04.h"

uint8_t TM_HCSR04_Init(TM_HCSR04_t* HCSR04, GPIO_TypeDef* ECHO_GPIOx, uint16_t ECHO_GPIO_Pin, GPIO_TypeDef* TRIGGER_GPIOx, uint16_t TRIGGER_GPIO_Pin) {	
	/* Init Delay functions */
	TM_DELAY_Init();
	
	/* Save everything */
	HCSR04->ECHO_GPIOx = ECHO_GPIOx;
	HCSR04->ECHO_GPIO_Pin = ECHO_GPIO_Pin;
	HCSR04->TRIGGER_GPIOx = TRIGGER_GPIOx;
	HCSR04->TRIGGER_GPIO_Pin = TRIGGER_GPIO_Pin;
	
	/* Initialize pins */	
	/* Trigger pin */
	TM_GPIO_Init(HCSR04->TRIGGER_GPIOx, HCSR04->TRIGGER_GPIO_Pin, TM_GPIO_Mode_OUT, TM_GPIO_OType_PP, TM_GPIO_PuPd_DOWN, TM_GPIO_Speed_Medium);
	
	/* Echo pin */
	TM_GPIO_Init(HCSR04->ECHO_GPIOx, HCSR04->ECHO_GPIO_Pin, TM_GPIO_Mode_IN, TM_GPIO_OType_PP, TM_GPIO_PuPd_DOWN, TM_GPIO_Speed_Medium);
	
	/* Trigger set to low */
	TM_GPIO_SetPinLow(HCSR04->TRIGGER_GPIOx, HCSR04->TRIGGER_GPIO_Pin);
	
	/* Start measurement, check if sensor is working */
	if (TM_HCSR04_Read(HCSR04) >= 0) {
		/* Sensor OK */
		return 1;
	}
	
	/* Sensor error */
	return 0;
}

float TM_HCSR04_Read(TM_HCSR04_t* HCSR04) {
	uint32_t time, timeout;
	/* Trigger low */
	TM_GPIO_SetPinLow(HCSR04->TRIGGER_GPIOx, HCSR04->TRIGGER_GPIO_Pin);
	/* Delay 2 us */
	Delay(2);
	/* Trigger high for 10us */
	TM_GPIO_SetPinHigh(HCSR04->TRIGGER_GPIOx, HCSR04->TRIGGER_GPIO_Pin);
	/* Delay 10 us */
	Delay(10);
	/* Trigger low */
	TM_GPIO_SetPinLow(HCSR04->TRIGGER_GPIOx, HCSR04->TRIGGER_GPIO_Pin);
	
	/* Give some time for response */
	timeout = HCSR04_TIMEOUT;
	while (!TM_GPIO_GetInputPinValue(HCSR04->ECHO_GPIOx, HCSR04->ECHO_GPIO_Pin)) {
		if (timeout-- == 0x00) {
			return -1;
		}
	}
	
	/* Start time */
	time = 0;
	/* Wait till signal is low */
	while (TM_GPIO_GetInputPinValue(HCSR04->ECHO_GPIOx, HCSR04->ECHO_GPIO_Pin)) {
		/* Increase time */
		time++;
		/* Delay 1us */
		Delay(1);
	}
	
	/* Convert us to cm */
	HCSR04->Distance =  (float)time * HCSR04_NUMBER;
	
	/* Return distance */
	return HCSR04->Distance;
}

/*
		Pin_Trigger	Pin_Echo
		D10			D9					SRF Depan Kanan
		C13	 		E6					SRF Depan Kiri
		E8			E7					SRF Serong Kanan
		E10			E9					SRF Serong Kiri
		A4			A5					SRF Kanan
		A9			A8					SRF Kiri
		B7			B6					SRF Depan Sementara
*/

TM_HCSR04_t SRFDepanKanan;
TM_HCSR04_t SRFDepanKiri;
TM_HCSR04_t SRFSerongKanan;
TM_HCSR04_t SRFSerongKiri;
TM_HCSR04_t SRFKanan;
TM_HCSR04_t SRFKiri;
TM_HCSR04_t SRFDepan;

float jarakDepanKiri;
float jarakDepanKanan;
float jarakSerongKiri;
float jarakSerongKanan;
float jarakKiri;
float jarakKanan;
float jarakDepan;

void init_srf()
{
	TM_HCSR04_Init(&SRFDepanKanan, GPIOD, GPIO_PIN_9, GPIOD, GPIO_PIN_10);
	TM_HCSR04_Init(&SRFDepanKiri, GPIOE, GPIO_PIN_6, GPIOC, GPIO_PIN_13);
//	TM_HCSR04_Init(&SRFSerongKanan, GPIOE, GPIO_PIN_7, GPIOE, GPIO_PIN_8);
//	TM_HCSR04_Init(&SRFSerongKiri, GPIOE, GPIO_PIN_9, GPIOE, GPIO_PIN_10);
	TM_HCSR04_Init(&SRFKanan, GPIOA, GPIO_PIN_5, GPIOA, GPIO_PIN_4);
	TM_HCSR04_Init(&SRFKiri, GPIOA, GPIO_PIN_8, GPIOA, GPIO_PIN_9);
	TM_HCSR04_Init(&SRFDepan, GPIOB, GPIO_PIN_6, GPIOB, GPIO_PIN_7);
}

void tesSRF(){
	jarakDepanKiri = TM_HCSR04_Read(&SRFDepanKanan);
	jarakDepanKanan = TM_HCSR04_Read(&SRFDepanKiri);
//	jarakSerongKiri = TM_HCSR04_Read(&SRFSerongKanan);
//	jarakSerongKanan = TM_HCSR04_Read(&SRFSerongKiri);
	jarakKiri = TM_HCSR04_Read(&SRFKanan);
	jarakKanan = TM_HCSR04_Read(&SRFKiri);
	jarakDepan = TM_HCSR04_Read(&SRFDepan);
}

