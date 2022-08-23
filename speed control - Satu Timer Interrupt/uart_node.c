#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_tim.h"
#include "stm32f4xx_usart.h"
#include "misc.h"
#include "uart_node.h"
#include <stdint.h>
#include "localization.h"

char gameState;
extern float movY;
extern float movX;
extern int movCompass;
char flagNode;
char bufferNode;
uint8_t ballX;
uint8_t ballY;
uint8_t ballKeeper;

void init_USART(void)
{
		NVIC_InitTypeDef NVIC_InitStructure;
		GPIO_InitTypeDef GPIO_InitStructure;
		USART_InitTypeDef USART_InitStructure;
		// sort out clocks
		RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOA, ENABLE);
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);

		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
		GPIO_Init(GPIOA, &GPIO_InitStructure);

		GPIO_PinAFConfig(GPIOA, GPIO_PinSource3 , GPIO_AF_USART2);
		GPIO_PinAFConfig(GPIOA, GPIO_PinSource2 , GPIO_AF_USART2);
		// Initialize USART
	    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);

	     USART_DeInit(USART2);
	     USART_InitStructure.USART_BaudRate = 9600;
	     USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	     USART_InitStructure.USART_StopBits = USART_StopBits_1;
	     USART_InitStructure.USART_Parity = USART_Parity_No;
	     USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	     USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	     USART_Init(USART2,&USART_InitStructure);

	     USART_Cmd(USART2, ENABLE);

	     /* Here the USART2 receive interrupt is enabled
	      * and the interrupt controller is configured
	      * to jump to the USART2_IRQHandler() function
	      * if the USART2 receive interrupt occurs
	      */
	     USART_ITConfig(USART2, USART_IT_RXNE, ENABLE); // enable the USART2 receive interrupt

	     NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;             // we want to configure the USART1 interrupts
	     NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;         // this sets the priority group of the USART1 interrupts
	     NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;        // this sets the subpriority inside the group
	     NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;           // the USART2 interrupts are globally enabled
	     NVIC_Init(&NVIC_InitStructure);                       // the properties are passed to the NVIC_Init function which takes care of the low level stuff

	     // finally this2, E enables the complete USART2 peripheral
	     USART_Cmd(USART2,ENABLE);

}

void USART2_IRQHandler(void){
	if( USART_GetITStatus(USART2, USART_IT_RXNE) )
	{
		bufferNode = USART_ReceiveData(USART2);
	}
	else{}
	if(flagNode=='M'){
		gameState = bufferNode;
		flagNode = 0;
	}
	else if(flagNode=='X'){
		ballX = bufferNode;
		flagNode = 0;
	}
	else if(flagNode=='B'){
		ballKeeper = bufferNode;
		flagNode = 0;
	}
	else
	{
		flagNode = bufferNode;
	}
//	USART_SendData(USART2, 7);
}

void init_node(){
init_USART();
}
