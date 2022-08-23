//--------------------------------------------------------------
// Original File     : stm32_ub_lcd_2x16.c
// After modified : lcd_16x2.c
// Date     : 15.04.2013
// Version  : 1.0
// Author   : UB //well, i don't know who he is but thanks!
// EMail    : mc-4u(@)t-online.de
// Web      : www.mikrocontroller-4u.de //thanks to them!
// CPU      : STM32F4
// IDE      : CooCox CoIDE 1.7.0
// Module   : GPIO
// Function : lcd 16x2
//            4Bit-Mode
//            Chip : ST7066U/HD44780/SED1278/KS0066U/S6A0069X
// 
// Basic pin configurations:
//             PE5  -> LCD_RS
//             PE6  -> LCD_E
//             PE7  -> LCD_DB4
//             PE8  -> LCD_DB5
//             PE9  -> LCD_DB6
//             PE10 -> LCD_DB7
//			   GND	-> LCD_R/W
//
//	modified and simplified by: Bintang
//--------------------------------------------------------------

/*	modifier's notes:
 *  -Some of the comments are still in German, and I don't want to bother myself to change them.
 *  -You can ignore every function marked by comment "internal function", but don't delete them
 *  -Here are some important functions that you might need:
 *  -lcd_clear(), lcd_putsf(int x, int y, char *str), lcd_gotoxy(int x, int y)
 *  -To begin with new project, I suggest you enable C library, Retarget printf, RCC and GPIO, M4 CMSIS Core, CMSIS BOOT
 */

//--------------------------------------------------------------
// Includes
//--------------------------------------------------------------
#include "lcd_16x2.h"



//--------------------------------------------------------------
// internal function
//--------------------------------------------------------------
void P_LCD_2x16_InitIO(void);
void P_LCD_2x16_PinLo(TLCD_NAME_t lcd_pin);
void P_LCD_2x16_PinHi(TLCD_NAME_t lcd_pin);
void P_LCD_2x16_Clk(void);
void P_LCD_2x16_InitSequenz(void);
void P_LCD_2x16_Cmd(uint8_t wert);
void P_LCD_2x16_Data(uint8_t wert);
void lcd_gotoxy(uint8_t x, uint8_t y);
void P_LCD_2x16_Delay(volatile uint32_t nCount);
 
//Variables
int menuKiri = 0;
int menuBack = 0;
int menuEnter = 0;
int menuKanan = 0;

int a = 0, b = 0;
int flagButton = 0;
char cetak[16];

//--------------------------------------------------------------
// definition of all pins for display
// Sequence as TLCD_NAME_t
//
// Init : [Bit_SET,Bit_RESET]
//--------------------------------------------------------------
LCD_2X16_t LCD_2X16[] = {
 // Name   ,PORT , PIN       , CLOCK              , Init
  {TLCD_RS ,GPIOE,GPIO_Pin_0 ,RCC_AHB1Periph_GPIOE,Bit_RESET}, //tadinya dari 5 - 10
  {TLCD_E  ,GPIOE,GPIO_Pin_1 ,RCC_AHB1Periph_GPIOE,Bit_RESET},
  {TLCD_D4 ,GPIOE,GPIO_Pin_2 ,RCC_AHB1Periph_GPIOE,Bit_RESET},
  {TLCD_D5 ,GPIOE,GPIO_Pin_3 ,RCC_AHB1Periph_GPIOE,Bit_RESET},
  {TLCD_D6 ,GPIOE,GPIO_Pin_4 ,RCC_AHB1Periph_GPIOE,Bit_RESET},
  {TLCD_D7 ,GPIOE,GPIO_Pin_5,RCC_AHB1Periph_GPIOE,Bit_RESET},
};



//--------------------------------------------------------------------
// initialize lcd function. Put this at the beginning of main function
//--------------------------------------------------------------------
void lcd_init(void)
{
  // init aller IO-Pins
  P_LCD_2x16_InitIO();
  // kleine Pause
  P_LCD_2x16_Delay(TLCD_INIT_PAUSE);
  // Init Sequenz starten
  P_LCD_2x16_InitSequenz();
  // LCD-Settings einstellen
  P_LCD_2x16_Cmd(TLCD_CMD_INIT_DISPLAY);
  P_LCD_2x16_Cmd(TLCD_CMD_ENTRY_MODE);
  // Display einschalten
  P_LCD_2x16_Cmd(TLCD_CMD_DISP_M1);
  // Display löschen
  P_LCD_2x16_Cmd(TLCD_CMD_CLEAR);
  // kleine Pause
  P_LCD_2x16_Delay(TLCD_PAUSE);
}


//--------------------------------------------------------------
// Clear everything displayed
//--------------------------------------------------------------
void lcd_clear(void)
{
  // Display löschen
  P_LCD_2x16_Cmd(TLCD_CMD_CLEAR);
  // kleine Pause
  P_LCD_2x16_Delay(TLCD_PAUSE);
}


//--------------------------------------------------------------
// Stellt einen Display Mode ein
// mode : [TLCD_OFF, TLCD_ON, TLCD_CURSOR, TLCD_BLINK]
//--------------------------------------------------------------
void UB_LCD_2x16_SetMode(TLCD_MODE_t mode)
{
  if(mode==TLCD_OFF) P_LCD_2x16_Cmd(TLCD_CMD_DISP_M0);
  if(mode==TLCD_ON) P_LCD_2x16_Cmd(TLCD_CMD_DISP_M1);
  if(mode==TLCD_CURSOR) P_LCD_2x16_Cmd(TLCD_CMD_DISP_M2);
  if(mode==TLCD_BLINK) P_LCD_2x16_Cmd(TLCD_CMD_DISP_M3);
}


//--------------------------------------------------------------
// to print string and place where it should be
// x : 0 to 15
// y : 0 to 1
//--------------------------------------------------------------
void lcd_putsf(uint8_t x, uint8_t y, char *ptr)
{
  // Cursor setzen
  lcd_gotoxy(x,y);
  // kompletten String ausgeben
  while (*ptr != 0) {
    P_LCD_2x16_Data(*ptr);
    ptr++;
  }
}


//--------------------------------------------------------------
// internal function
// initialize IO function
//--------------------------------------------------------------
void P_LCD_2x16_InitIO(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure;
  TLCD_NAME_t lcd_pin;
  
  for(lcd_pin=0;lcd_pin<TLCD_ANZ;lcd_pin++) {
    // Clock Enable
    RCC_AHB1PeriphClockCmd(LCD_2X16[lcd_pin].TLCD_CLK, ENABLE);

    // Config als Digital-Ausgang
    GPIO_InitStructure.GPIO_Pin = LCD_2X16[lcd_pin].TLCD_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(LCD_2X16[lcd_pin].TLCD_PORT, &GPIO_InitStructure);

    // Default Wert einstellen
    if(LCD_2X16[lcd_pin].TLCD_INIT==Bit_RESET) {
      P_LCD_2x16_PinLo(lcd_pin);
    }
    else {
      P_LCD_2x16_PinHi(lcd_pin);
    }
  }  
}


//--------------------------------------------------------------
// internal function
// Pin auf Lo setzen
//--------------------------------------------------------------
void P_LCD_2x16_PinLo(TLCD_NAME_t lcd_pin)
{
  LCD_2X16[lcd_pin].TLCD_PORT->BSRRH = LCD_2X16[lcd_pin].TLCD_PIN;
}


//--------------------------------------------------------------
// internal function
// Pin auf Hi setzen
//--------------------------------------------------------------
void P_LCD_2x16_PinHi(TLCD_NAME_t lcd_pin)
{
  LCD_2X16[lcd_pin].TLCD_PORT->BSRRL = LCD_2X16[lcd_pin].TLCD_PIN;
}


//--------------------------------------------------------------
// interne Funktion
// einen Clock Impuls ausgeben
//--------------------------------------------------------------
void P_LCD_2x16_Clk(void)
{
  // Pin-E auf Hi
  P_LCD_2x16_PinHi(TLCD_E);
  // kleine Pause
  P_LCD_2x16_Delay(TLCD_CLK_PAUSE);
  // Pin-E auf Lo
  P_LCD_2x16_PinLo(TLCD_E);
  // kleine Pause
  P_LCD_2x16_Delay(TLCD_CLK_PAUSE);  
}


//--------------------------------------------------------------
// interne Funktion
// init Sequenz für das Display
//--------------------------------------------------------------
void P_LCD_2x16_InitSequenz(void)
{
  // Init Sequenz
  P_LCD_2x16_PinHi(TLCD_D4);
  P_LCD_2x16_PinHi(TLCD_D5);
  P_LCD_2x16_PinLo(TLCD_D6);
  P_LCD_2x16_PinLo(TLCD_D7);
  // Erster Init Impuls
  P_LCD_2x16_Clk();
  P_LCD_2x16_Delay(TLCD_PAUSE);
  // Zweiter Init Impuls
  P_LCD_2x16_Clk();
  P_LCD_2x16_Delay(TLCD_PAUSE);
  // Dritter Init Impuls
  P_LCD_2x16_Clk();
  P_LCD_2x16_Delay(TLCD_PAUSE);
  // LCD-Modus einstellen (4Bit-Mode)
  P_LCD_2x16_PinLo(TLCD_D4);
  P_LCD_2x16_PinHi(TLCD_D5);
  P_LCD_2x16_PinLo(TLCD_D6);
  P_LCD_2x16_PinLo(TLCD_D7);
  P_LCD_2x16_Clk();
  P_LCD_2x16_Delay(TLCD_PAUSE);
}


//--------------------------------------------------------------
// internal function
// Kommando an das Display senden
//--------------------------------------------------------------
void P_LCD_2x16_Cmd(uint8_t wert)
{
  // RS=Lo (Command)
  P_LCD_2x16_PinLo(TLCD_RS);
  // Hi-Nibble ausgeben         
  if((wert&0x80)!=0) P_LCD_2x16_PinHi(TLCD_D7); else P_LCD_2x16_PinLo(TLCD_D7);
  if((wert&0x40)!=0) P_LCD_2x16_PinHi(TLCD_D6); else P_LCD_2x16_PinLo(TLCD_D6);
  if((wert&0x20)!=0) P_LCD_2x16_PinHi(TLCD_D5); else P_LCD_2x16_PinLo(TLCD_D5);
  if((wert&0x10)!=0) P_LCD_2x16_PinHi(TLCD_D4); else P_LCD_2x16_PinLo(TLCD_D4);
  P_LCD_2x16_Clk();
  // Lo-Nibble ausgeben         
  if((wert&0x08)!=0) P_LCD_2x16_PinHi(TLCD_D7); else P_LCD_2x16_PinLo(TLCD_D7);
  if((wert&0x04)!=0) P_LCD_2x16_PinHi(TLCD_D6); else P_LCD_2x16_PinLo(TLCD_D6);
  if((wert&0x02)!=0) P_LCD_2x16_PinHi(TLCD_D5); else P_LCD_2x16_PinLo(TLCD_D5);
  if((wert&0x01)!=0) P_LCD_2x16_PinHi(TLCD_D4); else P_LCD_2x16_PinLo(TLCD_D4);
  P_LCD_2x16_Clk();  
}


//--------------------------------------------------------------
// internal function
// Daten an das Display senden
//--------------------------------------------------------------
void P_LCD_2x16_Data(uint8_t wert)
{
  // RS=Hi (Data)
  P_LCD_2x16_PinHi(TLCD_RS);
  // Hi-Nibble ausgeben          
  if((wert&0x80)!=0) P_LCD_2x16_PinHi(TLCD_D7); else P_LCD_2x16_PinLo(TLCD_D7);
  if((wert&0x40)!=0) P_LCD_2x16_PinHi(TLCD_D6); else P_LCD_2x16_PinLo(TLCD_D6);
  if((wert&0x20)!=0) P_LCD_2x16_PinHi(TLCD_D5); else P_LCD_2x16_PinLo(TLCD_D5);
  if((wert&0x10)!=0) P_LCD_2x16_PinHi(TLCD_D4); else P_LCD_2x16_PinLo(TLCD_D4);
  P_LCD_2x16_Clk();
  // Lo-Nibble ausgeben        
  if((wert&0x08)!=0) P_LCD_2x16_PinHi(TLCD_D7); else P_LCD_2x16_PinLo(TLCD_D7);
  if((wert&0x04)!=0) P_LCD_2x16_PinHi(TLCD_D6); else P_LCD_2x16_PinLo(TLCD_D6);
  if((wert&0x02)!=0) P_LCD_2x16_PinHi(TLCD_D5); else P_LCD_2x16_PinLo(TLCD_D5);
  if((wert&0x01)!=0) P_LCD_2x16_PinHi(TLCD_D4); else P_LCD_2x16_PinLo(TLCD_D4);
  P_LCD_2x16_Clk();  
}


//--------------------------------------------------------------
// internal function
// Cursor auf x,y stellen
//--------------------------------------------------------------
void lcd_gotoxy(uint8_t x, uint8_t y)
{
  uint8_t wert;

  if(x>=TLCD_MAXX) x=0;
  if(y>=TLCD_MAXY) y=0;

  wert=(y<<6);
  wert|=x;
  wert|=0x80;
  P_LCD_2x16_Cmd(wert);
}


//--------------------------------------------------------------
// kleine Pause (ohne Timer)
//--------------------------------------------------------------
void P_LCD_2x16_Delay(volatile uint32_t nCount)
{
  while(nCount--)
  {
  }
}

void init_menu()
{
	GPIO_InitTypeDef gpio_init;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);

	gpio_init.GPIO_Pin  = GPIO_Pin_5 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9;
	gpio_init.GPIO_Mode = GPIO_Mode_IN;
	gpio_init.GPIO_Speed = GPIO_Speed_100MHz;
	gpio_init.GPIO_OType = GPIO_OType_PP;
	gpio_init.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_Init(GPIOC, &gpio_init);

}

void menu()
{
	menuKiri = GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_7);
	menuBack = GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_8);
	menuEnter = GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_9);
	menuKanan = GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_5);
	if(b == 0)
	{
		if(flagButton == 0)
		{
			if(menuKiri == 1)
			{
				a--;
				if(a < 0)
				{
					a = 0;
				}
				flagButton = 1;
			}
			if(menuEnter == 1)
			{
				b++;
				if(b > 1)
				{
					b = 1;
				}
				flagButton = 1;
			}
			if(menuKanan == 1)
			{
				a++;
				if(a > 2)
				{
					a = 2;
				}
				flagButton = 1;
			}
		}
		else{}
		if(menuKiri == 0 && menuBack == 0 && menuEnter == 0 && menuKanan == 0)
		{
			flagButton = 0;
		}
	}
	else if(b > 0)
	{
		if(menuBack == 1)
		{
			b--;
			if(b < 0)
			{
				b = 0;
			}
			menuBack = 0;
		}
		if(a == 0)
		{
			while(1)
			{
				tesMotor();
				if(menuBack == 1)
				{
					break;
				}
			}
		}
		else if(a == 1)
		{
			while(1)
			{
				tesHandle();
				menuBack = GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_8);
				if(menuBack == 1)
				{
					break;
				}
			}
		}
		else if(a == 2)
		{
			while(1)
			{
				getBallNew();
				if(menuBack == 1)
				{
					b--;
					break;
				}
			}
		}
		else if(a == 2)
		{
			while(1)
			{
				getBallNew();
				if(menuBack == 1)
				{
					b--;
					break;
				}
			}
		}
	}



}

void printMenu(){
	lcd_clear();
	if(a == 0)
	{
		sprintf(cetak, "tesMotor");
	}
	else if(a == 1)
	{
		sprintf(cetak, "tesHandle");
	}
	else if(a == 2)
	{
		sprintf(cetak, "getBallNew");
	}
	lcd_putsf(0,0,cetak);
	sprintf(cetak, "KI   BA   EN  KA");
	lcd_putsf(0,1,cetak);
}
