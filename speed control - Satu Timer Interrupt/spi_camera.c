//#include "stm32f4xx_gpio.h"
//#include "stm32f4xx_rcc.h"
//#include "stm32f4xx_tim.h"
//#include <stm32f4xx.h>
//#include <stm32f4xx_spi.h>
//#include "misc.h"
//
//uint8_t ballX = 0;
//uint8_t ballY = 0;
//
//// this function initializes the SPI3 peripheral
//void init_SPI3(void)
//{
////  NVIC_InitTypeDef NVIC_InitStructure;
//	GPIO_InitTypeDef GPIO_InitStruct;
//		SPI_InitTypeDef SPI_InitStruct;
//
//		// enable clock for used IO pins
//		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
//
//		/* configure pins used by SPI1
//		 * PA5 = SCK
//		 * PA6 = MISO
//		 * PA7 = MOSI
//		 */
//		GPIO_InitStruct.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_11 | GPIO_Pin_10;
//		GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
//		GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
//		GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
//		GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
//		GPIO_Init(GPIOC, &GPIO_InitStruct);
//
//		// connect SPI1 pins to SPI alternate function
//		GPIO_PinAFConfig(GPIOC, GPIO_PinSource10, GPIO_AF_SPI3);
//		GPIO_PinAFConfig(GPIOC, GPIO_PinSource11, GPIO_AF_SPI3);
//		GPIO_PinAFConfig(GPIOC, GPIO_PinSource12, GPIO_AF_SPI3);
//
//		// enable clock for used IO pins
//		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
//
//		/* Configure the chip select pin
//		   in this case we will use PE7 */
//		GPIO_InitStruct.GPIO_Pin = GPIO_Pin_7;
//		GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
//		GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
//		GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
//		GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
//		GPIO_Init(GPIOE, &GPIO_InitStruct);
//
//		GPIOE->BSRRL |= GPIO_Pin_7; // set PE7 high
//
//		// enable peripheral clock
//		RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI3, ENABLE);
//
//		/* configure SPI1 in Mode 0
//		 * CPOL = 0 --> clock is low when idle
//		 * CPHA = 0 --> data is sampled at the first edge
//		 */
//		SPI_InitStruct.SPI_Direction = SPI_Direction_2Lines_FullDuplex; // set to full duplex mode, seperate MOSI and MISO lines
//		SPI_InitStruct.SPI_Mode = SPI_Mode_Master;     // transmit in master mode, NSS pin has to be always high
//		SPI_InitStruct.SPI_DataSize = SPI_DataSize_8b; // one packet of data is 8 bits wide
//		SPI_InitStruct.SPI_CPOL = SPI_CPOL_Low;        // clock is low when idle
//		SPI_InitStruct.SPI_CPHA = SPI_CPHA_1Edge;      // data sampled at first edge
//		SPI_InitStruct.SPI_NSS = SPI_NSS_Soft | SPI_NSSInternalSoft_Set; // set the NSS management to internal and pull internal NSS high
//		SPI_InitStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4; // SPI frequency is APB2 frequency / 4
//		SPI_InitStruct.SPI_FirstBit = SPI_FirstBit_MSB;// data is transmitted MSB first
//		SPI_Init(SPI3, &SPI_InitStruct);
//
////		SPI_Cmd(SPI1, ENABLE); // enable SPI1
//
//
////  SPI_Cmd(SPI1, ENABLE);
//
////  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
////  NVIC_InitStructure.NVIC_IRQChannel = SPI3_IRQn;
////  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
////  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
////  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
////  NVIC_Init(&NVIC_InitStructure);
//
//  /* Enable SPI1*/
//  SPI_Cmd(SPI3, ENABLE);
//  return;
//}
//
///* This funtion is used to transmit and receive data
// * with SPI3
// * 			data --> data to be transmitted
// * 			returns received value
// */
//
////void SPI3_IRQHandler(void)
////{
////    uint8_t buffer;
////    static unsigned short int count = 0, i = 0 ;
////    if (SPI_I2S_GetITStatus(SPI3, SPI_I2S_IT_RXNE) == SET)
////    {
////    	buffer = SPI_I2S_ReceiveData(SPI3);
////        ballX = buffer;
////        i++ ;
////
////    }
////}
//
////void write_SPI3(int data)
////{
//////	SPI_I2S_ITConfig(SPI3,SPI_I2S_IT_TXE,ENABLE);
//////	while(SPI_I2S_GetFlagStatus(SPI3, SPI_I2S_FLAG_TXE)!=SET);
////	while(((SPI3)->SR & (SPI_SR_TXE | SPI_SR_RXNE)) == 0 || ((SPI3)->SR & SPI_SR_BSY));
////	SPI_I2S_SendData(SPI3,data);
////}
//
//uint8_t SPI3_send(uint8_t data){
//
//
//
//	SPI3->DR = data; // write data to be transmitted to the SPI data register
//
//	while( !(SPI3->SR & SPI_I2S_FLAG_TXE) ); // wait until transmit complete
//
//	while( !(SPI3->SR & SPI_I2S_FLAG_RXNE) ); // wait until receive complete
//
//	while( SPI3->SR & SPI_I2S_FLAG_BSY ); // wait until SPI is not busy anymore
//
//	return SPI3->DR; // return received data from SPI data register
//
//}
////
////uint8_t SPI3_get(uint8_t data){
////
////	SPI3->DR = data; // write data to be transmitted to the SPI data register
////	(SPI3->SR & SPI_I2S_FLAG_TXE); // wait until transmit complete
//////	(SPI3->SR & SPI_I2S_FLAG_RXNE); // wait until receive complete
//////	while( SPI3->SR & SPI_I2S_FLAG_BSY ); // wait until SPI is not busy anymore
////	return SPI3->DR; // return received data from SPI data register
////}
////int main(void){
////
////	uint8_t received_val = 0;
////
////	init_SPI3();
////
////	while(1){
////
////		GPIOE->BSRRH |= GPIO_Pin_7; // set PE7 (CS) low
////		SPI3_send(0xAA);  // transmit data
////		received_val = SPI3_send(0x00); // transmit dummy byte and receive data
////		GPIOE->BSRRL |= GPIO_Pin_7; // set PE7 (CS) high
////	}
////}
