#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_tim.h"
#include "stm32f4xx_usart.h"
#include "misc.h"

#define SUDUTGAWANG 115

uint8_t ballXCoor;
uint8_t ballYCoor;
int compassHeading;
uint8_t compassDerajat;
char flag;
uint8_t ballFound;

void init_camera(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
		GPIO_InitTypeDef GPIO_InitStructure;
		USART_InitTypeDef USART_InitStructure;
		// sort out clocks
		RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOB, ENABLE);
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
		/* Configure USART3 Tx (PB.10) as alternate function push-pull */
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
		GPIO_Init(GPIOB, &GPIO_InitStructure);
		// Map USART3 to B.10
		GPIO_PinAFConfig(GPIOB, GPIO_PinSource10, GPIO_AF_USART3);
		GPIO_PinAFConfig(GPIOB, GPIO_PinSource11 , GPIO_AF_USART3);
		// Initialize USART
		USART_InitStructure.USART_BaudRate = 9600;
		USART_InitStructure.USART_WordLength = USART_WordLength_8b;
		USART_InitStructure.USART_StopBits = USART_StopBits_1;
		USART_InitStructure.USART_Parity = USART_Parity_No;
		USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
		USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
		/* Configure USART */
		USART_Init(USART3, &USART_InitStructure);
		/* Enable the USART */
		USART_Cmd(USART3, ENABLE);

	     /* Here the USART3 receive interrupt is enabled
	      * and the interrupt controller is configured
	      * to jump to the USART3_IRQHandler() function
	      * if the USART3 receive interrupt occurs
	      */
	     USART_ITConfig(USART3, USART_IT_RXNE, ENABLE); // enable the USART3 receive interrupt

	     NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;             // we want to configure the USART3 interrupts
	     NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;         // this sets the priority group of the USAR31 interrupts
	     NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;        // this sets the subpriority inside the group
	     NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;           // the USART3 interrupts are globally enabled
	     NVIC_Init(&NVIC_InitStructure);                       // the properties are passed to the NVIC_Init function which takes care of the low level stuff

	     // finally this2, E enables the complete USART3 peripheral
	     USART_Cmd(USART3,ENABLE);
}

void USART3_IRQHandler(void){
	uint8_t buffer;
	// check if the USART1 receive interrupt flag was set
	if( USART_GetITStatus(USART3, USART_IT_RXNE) ){
		buffer = USART_ReceiveData(USART3);
	}
	else{}
	if(flag=='X'){
		ballXCoor = buffer;
		flag = 0;
	}
	else if(flag=='Y'){
		ballYCoor = buffer;
		flag = 0;
	}
	else if (flag =='C'){
		compassDerajat = buffer;
		compassHeading = compassDerajat*360/255;
		compassHeading = compassHeading - SUDUTGAWANG;
		if(compassHeading < 0)
		{
			compassHeading = compassHeading + 360;
		}
		flag = 0;
	}
	else if(flag == 'B'){
		ballFound = buffer;
		flag = 0;
	}
	else
	{
		flag = buffer;
	}
}
