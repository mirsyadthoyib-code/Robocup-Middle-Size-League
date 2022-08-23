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
 
 // This is the modified library, added another motor so that it can read 2 motor encoders at the same time
 // @Adika Bintang Sulaeman

 // 1 jan 2017 library Update :
 // add 2 another motor encoder
 // @Yunus Maulana

 // 24 oct 2017 library Update :
 // add 1 kicker motor encoder
 // @Muhammad Irsyad Thoyib

#include "tm_stm32f4_rotary_encoder.h"

int32_t TM_INT_RE_Count;
int32_t secondCount;
int32_t thirdCount;
int32_t fourthCount;
int32_t fifthCount;
int32_t sixthCount;
int32_t kickerCount;

void TM_RE_Init(TM_RE_t* data) {
	GPIO_InitTypeDef GPIO_InitStruct;
	EXTI_InitTypeDef EXTI_InitStruct;
	NVIC_InitTypeDef NVIC_InitStruct;
	
	/* Pins as inputs */
	RCC_AHB1PeriphClockCmd(TM_RE_A_RCC | TM_RE_B_RCC, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
	
	/* Pin common settings */
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
	
	/* A pin */
	GPIO_InitStruct.GPIO_Pin = TM_RE_A_PIN;
	GPIO_Init(TM_RE_A_PORT, &GPIO_InitStruct);
	/* B pin */
	GPIO_InitStruct.GPIO_Pin = TM_RE_B_PIN;
	GPIO_Init(TM_RE_B_PORT, &GPIO_InitStruct);
	
	/* Connect pin to interrupt */
	SYSCFG_EXTILineConfig(TM_RE_A_EXTIPORTSOURCE, TM_RE_A_EXTIPINSOURCE);
	
	/* Configure external interrupt */
	EXTI_InitStruct.EXTI_Line = TM_RE_A_EXTI_LINE;	
	EXTI_InitStruct.EXTI_LineCmd = ENABLE;
	EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
	EXTI_Init(&EXTI_InitStruct);

	/* Add interrupt to NVIC */
	NVIC_InitStruct.NVIC_IRQChannel = TM_RE_A_NVIC;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = RE_NVIC_PRIORITY;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = RE_NVIC_SUBPRIORITY;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStruct);
	
	/* Set default */
	TM_INT_RE_Count = 0;
	data->Diff = 0;
	data->Absolute = 0;
}

void InitSecondEncoder(TM_RE_t* data) {
	GPIO_InitTypeDef GPIO_InitStruct;
	EXTI_InitTypeDef EXTI_InitStruct;
	NVIC_InitTypeDef NVIC_InitStruct;

	/* Pins as inputs */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD | RCC_AHB1Periph_GPIOD, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

	/* Pin common settings */
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;

	/* A pin */
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_1;
	GPIO_Init(GPIOD, &GPIO_InitStruct);
	/* B pin */
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_5;
	GPIO_Init(GPIOD, &GPIO_InitStruct);

	/* Connect pin to interrupt */
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOD, EXTI_PinSource1);

	/* Configure external interrupt */
	EXTI_InitStruct.EXTI_Line = EXTI_Line1;
	EXTI_InitStruct.EXTI_LineCmd = ENABLE;
	EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
	EXTI_Init(&EXTI_InitStruct);

	/* Add interrupt to NVIC */
	NVIC_InitStruct.NVIC_IRQChannel = EXTI1_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0x03;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0x00;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStruct);

	/* Set default */
	secondCount = 0;
	data->Diff = 0;
	data->Absolute = 0;
}

void InitThirdEncoder(TM_RE_t* data) {
	GPIO_InitTypeDef GPIO_InitStruct;
	EXTI_InitTypeDef EXTI_InitStruct;
	NVIC_InitTypeDef NVIC_InitStruct;

	/* Pins as inputs */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD | RCC_AHB1Periph_GPIOD, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

	/* Pin common settings */
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;

	/* A pin */
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_2;
	GPIO_Init(GPIOD, &GPIO_InitStruct);
	/* B pin */
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6;
	GPIO_Init(GPIOD, &GPIO_InitStruct);

	/* Connect pin to interrupt */
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOD, EXTI_PinSource2);


	/* Configure external interrupt */
	EXTI_InitStruct.EXTI_Line = EXTI_Line2;
	EXTI_InitStruct.EXTI_LineCmd = ENABLE;
	EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
	EXTI_Init(&EXTI_InitStruct);

	/* Add interrupt to NVIC */
	NVIC_InitStruct.NVIC_IRQChannel = EXTI2_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0x03;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0x00;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStruct);

	/* Set default */
	thirdCount = 0;
	data->Diff = 0;
	data->Absolute = 0;
}

void InitFourthEncoder(TM_RE_t* data) {
	GPIO_InitTypeDef GPIO_InitStruct;
	EXTI_InitTypeDef EXTI_InitStruct;
	NVIC_InitTypeDef NVIC_InitStruct;

	/* Pins as inputs */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD | RCC_AHB1Periph_GPIOD, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

	/* Pin common settings */
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;

	/* A pin */
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_3;
	GPIO_Init(GPIOD, &GPIO_InitStruct);
	/* B pin */
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_7;
	GPIO_Init(GPIOD, &GPIO_InitStruct);

	/* Connect pin to interrupt */
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOD, EXTI_PinSource3);

	/* Configure external interrupt */
	EXTI_InitStruct.EXTI_Line = EXTI_Line3;
	EXTI_InitStruct.EXTI_LineCmd = ENABLE;
	EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
	EXTI_Init(&EXTI_InitStruct);

	/* Add interrupt to NVIC */
	NVIC_InitStruct.NVIC_IRQChannel = EXTI3_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0x03;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0x00;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStruct);

	/* Set default */
	fourthCount = 0;
	data->Diff = 0;
	data->Absolute = 0;
}

void InitFifthEncoder(TM_RE_t* data) {
	GPIO_InitTypeDef GPIO_InitStruct;
	EXTI_InitTypeDef EXTI_InitStruct;
	NVIC_InitTypeDef NVIC_InitStruct;

	/* Pins as inputs */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB , ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

	/* Pin common settings */
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;

	/* A pin */
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_4;
	GPIO_Init(GPIOB, &GPIO_InitStruct);
	/* B pin */
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_2;
	GPIO_Init(GPIOB, &GPIO_InitStruct);

	/* Connect pin to interrupt */
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB, EXTI_PinSource4);

	/* Configure external interrupt */
	EXTI_InitStruct.EXTI_Line = EXTI_Line4;
	EXTI_InitStruct.EXTI_LineCmd = ENABLE;
	EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
	EXTI_Init(&EXTI_InitStruct);

	/* Add interrupt to NVIC */
	NVIC_InitStruct.NVIC_IRQChannel = EXTI4_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0x03;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0x00;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStruct);

	/* Set default */
	fifthCount = 0;
	data->Diff = 0;
	data->Absolute = 0;
}

void InitSixthEncoder(TM_RE_t* data) {
	GPIO_InitTypeDef GPIO_InitStruct;
	EXTI_InitTypeDef EXTI_InitStruct;
	NVIC_InitTypeDef NVIC_InitStruct;

	/* Pins as inputs */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB , ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

	/* Pin common settings */
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;

	/* A pin */
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_5;
	GPIO_Init(GPIOB, &GPIO_InitStruct);
	/* B pin */
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_3;
	GPIO_Init(GPIOB, &GPIO_InitStruct);

	/* Connect pin to interrupt */
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB, EXTI_PinSource5);

	/* Configure external interrupt */
	EXTI_InitStruct.EXTI_Line = EXTI_Line5;
	EXTI_InitStruct.EXTI_LineCmd = ENABLE;
	EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
	EXTI_Init(&EXTI_InitStruct);

	/* Add interrupt to NVIC */
	NVIC_InitStruct.NVIC_IRQChannel = EXTI9_5_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0x03;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0x00;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStruct);

	/* Set default */
	sixthCount = 0;
	data->Diff = 0;
	data->Absolute = 0;
}

void InitKickerEncoder(TM_RE_t* data) {
	GPIO_InitTypeDef GPIO_InitStruct;
	EXTI_InitTypeDef EXTI_InitStruct;
	NVIC_InitTypeDef NVIC_InitStruct;

	/* Pins as inputs */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD | RCC_AHB1Periph_GPIOD, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

	/* Pin common settings */
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;

	/* A pin */
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9;
	GPIO_Init(GPIOD, &GPIO_InitStruct);
	/* B pin */
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_8;
	GPIO_Init(GPIOD, &GPIO_InitStruct);
	/* Connect pin to interrupt */
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOD, EXTI_PinSource9);

	/* Configure external interrupt */
	EXTI_InitStruct.EXTI_Line = EXTI_Line9;
	EXTI_InitStruct.EXTI_LineCmd = ENABLE;
	EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
	EXTI_Init(&EXTI_InitStruct);

	/* Add interrupt to NVIC */
	NVIC_InitStruct.NVIC_IRQChannel = EXTI9_5_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0x03;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0x00;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStruct);

	/* Set default */
	kickerCount = 0;
	data->Diff = 0;
	data->Absolute = 0;
}

TM_RE_Rotate_t TM_RE_Get(TM_RE_t* data) {
	data->Diff = TM_INT_RE_Count - data->Absolute;
	data->Absolute = TM_INT_RE_Count;
	
	if (TM_INT_RE_Count < 0) {
		data->Rotation = TM_RE_Rotate_Decrement;
		return TM_RE_Rotate_Decrement;
	} else if (TM_INT_RE_Count > 0) {
		data->Rotation = TM_RE_Rotate_Increment;
		return TM_RE_Rotate_Increment;
	}
	data->Rotation = TM_RE_Rotate_Nothing;
	return TM_RE_Rotate_Nothing;
}

TM_RE_Rotate_t SecondGet(TM_RE_t* data) {
	data->Diff = secondCount - data->Absolute;
	data->Absolute = secondCount;

	if (secondCount < 0) {
		data->Rotation = TM_RE_Rotate_Decrement;
		return TM_RE_Rotate_Decrement;
	} else if (secondCount > 0) {
		data->Rotation = TM_RE_Rotate_Increment;
		return TM_RE_Rotate_Increment;
	}
	data->Rotation = TM_RE_Rotate_Nothing;
	return TM_RE_Rotate_Nothing;
}

TM_RE_Rotate_t ThirdGet(TM_RE_t* data) {
	data->Diff = thirdCount - data->Absolute;
	data->Absolute = thirdCount;

	if (thirdCount < 0) {
		data->Rotation = TM_RE_Rotate_Decrement;
		return TM_RE_Rotate_Decrement;
	} else if (thirdCount > 0) {
		data->Rotation = TM_RE_Rotate_Increment;
		return TM_RE_Rotate_Increment;
	}
	data->Rotation = TM_RE_Rotate_Nothing;
	return TM_RE_Rotate_Nothing;
}

TM_RE_Rotate_t FourthGet(TM_RE_t* data) {
	data->Diff = fourthCount - data->Absolute;
	data->Absolute = fourthCount;

	if (fourthCount < 0) {
		data->Rotation = TM_RE_Rotate_Decrement;
		return TM_RE_Rotate_Decrement;
	} else if (fourthCount > 0) {
		data->Rotation = TM_RE_Rotate_Increment;
		return TM_RE_Rotate_Increment;
	}
	data->Rotation = TM_RE_Rotate_Nothing;
	return TM_RE_Rotate_Nothing;
}

TM_RE_Rotate_t FifthGet(TM_RE_t* data) {
	data->Diff = fifthCount - data->Absolute;
	data->Absolute = fifthCount;

	if (fifthCount < 0) {
		data->Rotation = TM_RE_Rotate_Decrement;
		return TM_RE_Rotate_Decrement;
	} else if (fifthCount > 0) {
		data->Rotation = TM_RE_Rotate_Increment;
		return TM_RE_Rotate_Increment;
	}
	data->Rotation = TM_RE_Rotate_Nothing;
	return TM_RE_Rotate_Nothing;
}

TM_RE_Rotate_t SixthGet(TM_RE_t* data) {
	data->Diff = sixthCount - data->Absolute;
	data->Absolute = sixthCount;

	if (sixthCount < 0) {
		data->Rotation = TM_RE_Rotate_Decrement;
		return TM_RE_Rotate_Decrement;
	} else if (sixthCount > 0) {
		data->Rotation = TM_RE_Rotate_Increment;
		return TM_RE_Rotate_Increment;
	}
	data->Rotation = TM_RE_Rotate_Nothing;
	return TM_RE_Rotate_Nothing;
}

TM_RE_Rotate_t KickerGet(TM_RE_t* data) {
	data->Diff = kickerCount - data->Absolute;
	data->Absolute = kickerCount;

	if (kickerCount < 0) {
			data->Rotation = TM_RE_Rotate_Decrement;
			return TM_RE_Rotate_Decrement;
		} else if (kickerCount > 0) {
			data->Rotation = TM_RE_Rotate_Increment;
			return TM_RE_Rotate_Increment;
	}
	data->Rotation = TM_RE_Rotate_Nothing;
	return TM_RE_Rotate_Nothing;
}

void TM_RE_Process(void) {
	static uint8_t last_a = 1;
	uint8_t now_a;
	uint8_t now_b;
	
	now_a = GPIO_ReadInputDataBit(TM_RE_A_PORT, TM_RE_A_PIN);
	now_b = GPIO_ReadInputDataBit(TM_RE_B_PORT, TM_RE_B_PIN);
	if (now_a != last_a) {
		last_a = now_a;
		
		if (last_a == 0) {
#ifdef TM_RE_CHANGE_ROTATION
			if (now_b == 1) {
				TM_INT_RE_Count--;
			} else {
				TM_INT_RE_Count++;
			}
#else
			if (now_b == 1) {
				TM_INT_RE_Count++;
			} else {
				TM_INT_RE_Count--;
			}
#endif
		}
	}
}

void SecondProcess(void) {
	static uint8_t last_a = 1;
	uint8_t now_a;
	uint8_t now_b;

	now_a = GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_1);
	now_b = GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_5);
	if (now_a != last_a) {
		last_a = now_a;

		if (last_a == 0) {
#ifdef TM_RE_CHANGE_ROTATION
			if (now_b == 1) {
				secondCount--;
			} else {
				secondCount++;
			}
#else
			if (now_b == 1) {
				secondCount++;
			} else {
				secondCount--;
			}
#endif
		}
	}
}

void ThirdProcess(void) {
	static uint8_t last_a = 1;
	uint8_t now_a;
	uint8_t now_b;

	now_a = GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_2);
	now_b = GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_6);
	if (now_a != last_a) {
		last_a = now_a;

		if (last_a == 0) {
#ifdef TM_RE_CHANGE_ROTATION
			if (now_b == 1) {
				thirdCount--;
			} else {
				thirdCount++;
			}
#else
			if (now_b == 1) {
				thirdCount++;
			} else {
				thirdCount--;
			}
#endif
		}
	}
}

void FourthProcess(void) {
	static uint8_t last_a = 1;
	uint8_t now_a;
	uint8_t now_b;

	now_a = GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_3);
	now_b = GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_7);
	if (now_a != last_a) {
		last_a = now_a;

		if (last_a == 0) {
#ifdef TM_RE_CHANGE_ROTATION
			if (now_b == 1) {
				fourthCount--;
			} else {
				fourthCount++;
			}
#else
			if (now_b == 1) {
				fourthCount++;
			} else {
				fourthCount--;
			}
#endif
		}
	}
}

void FifthProcess(void) {
	static uint8_t last_a = 1;
	uint8_t now_a;
	uint8_t now_b;

	now_a = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_4);
	now_b = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_2);
	if (now_a != last_a) {
		last_a = now_a;

		if (last_a == 0) {
#ifdef TM_RE_CHANGE_ROTATION
			if (now_b == 1) {
				fifthCount--;
			} else {
				fifthCount++;
			}
#else
			if (now_b == 1) {
				fifthCount++;
			} else {
				fifthCount--;
			}
#endif
		}
	}
}

void SixthProcess(void) {
	static uint8_t last_a = 1;
	uint8_t now_a;
	uint8_t now_b;

	now_a = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_5);
	now_b = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_3);
	if (now_a != last_a) {
		last_a = now_a;

		if (last_a == 0) {
#ifdef TM_RE_CHANGE_ROTATION
			if (now_b == 1) {
				sixthCount--;
			} else {
				sixthCount++;
			}
#else
			if (now_b == 1) {
				sixthCount++;
			} else {
				sixthCount--;
			}
#endif
		}
	}
}

void KickerProcess(void) {
	static uint8_t last_a = 1;
	uint8_t now_a;
	uint8_t now_b;

	now_a = GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_8);
	now_b = GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_9);
	if (now_a != last_a) {
		last_a = now_a;

		if (last_a == 0) {
#ifdef TM_RE_CHANGE_ROTATION
			if (now_b == 1) {
				kickerCount--;
			} else {
				kickerCount++;
			}
#else
			if (now_b == 1) {
				kickerCount++;
			} else {
				kickerCount--;
			}
#endif
		}
	}
}


#ifdef TM_RE_A_DEFAULT
/* Default configuration is in use */
void EXTI0_IRQHandler(void) {
	if (EXTI_GetITStatus(TM_RE_A_EXTI_LINE) != RESET) {
		/* Process rotary encoder */
		TM_RE_Process();
		
		/* Clear interrupt bit */
		EXTI_ClearITPendingBit(TM_RE_A_EXTI_LINE);
	}
}
#endif

#ifdef ACTIVATESECMOTOR
void EXTI1_IRQHandler(void) {
	if (EXTI_GetITStatus(EXTI_Line1) != RESET) {
		/* Process rotary encoder */
		SecondProcess();

		/* Clear interrupt bit */
		EXTI_ClearITPendingBit(EXTI_Line1);
	}
}

#endif

#ifdef ACTIVATETHCMOTOR
void EXTI2_IRQHandler(void) {
	if (EXTI_GetITStatus(EXTI_Line2) != RESET) {
		/* Process rotary encoder */
		ThirdProcess();

		/* Clear interrupt bit */
		EXTI_ClearITPendingBit(EXTI_Line2);
	}
}

#endif

#ifdef ACTIVATEFOCMOTOR
void EXTI3_IRQHandler(void) {
	if (EXTI_GetITStatus(EXTI_Line3) != RESET) {
		/* Process rotary encoder */
		FourthProcess();

		/* Clear interrupt bit */
		EXTI_ClearITPendingBit(EXTI_Line3);
	}
}

#endif

#ifdef ACTIVATEFICMOTOR
void EXTI4_IRQHandler(void) {
	if (EXTI_GetITStatus(EXTI_Line4) != RESET) {
		/* Process rotary encoder */
		FifthProcess();

		/* Clear interrupt bit */
		EXTI_ClearITPendingBit(EXTI_Line4);
	}
}

#endif
//
//#ifdef ACTIVATESICMOTOR
//void EXTI9_5_IRQHandler() {
//	if (EXTI_GetITStatus(EXTI_Line5) != RESET) {
//		/* Process rotary encoder */
//		SixthProcess();
//
//		/* Clear interrupt bit */
//		EXTI_ClearITPendingBit(EXTI_Line5);
//	}
//}
//
//#endif


#ifdef ACTIVATEKICMOTOR
void EXTI9_5_IRQHandler(void) {
	if (EXTI_GetITStatus(EXTI_Line9) != RESET) {
		/* Process rotary encoder */
		KickerProcess();

		/* Clear interrupt bit */
		EXTI_ClearITPendingBit(EXTI_Line9);
	}
}

#endif
