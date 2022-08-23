#ifndef LOCALIZATION_H_
#define LOCALIZATION_H_

#include "speed_control.h"
#include "pwm_motor.h"
#include <math.h>
#include "uart_node.h"
#include "uart_camera.h"
#include "uart_line.h"
#include "tm_stm32f4_delay.h"
#include "stdbool.h"

#define servoMid 125//112  // 142
#define goalX  12
#define goalY  -3
#define IHead 0
#define DHead 0
#define PHead 1
#define ON 1
#define OFF 0
#define PI 3.14159265

/*
 * Update Note 8 Februari 2017
 * - heading merupakan arah robot menghadap yang di acukan saat robot baru dinyalakan dalam satuan derajat(0-360)
 * - perhitungan heading dimulai dengan menandakan mulainya perhitungan(startHeading =1) jika robot bergerak rotasi
 * yang dituliskan pada fungsi rotateClockWise dan rotateAntiClockWise di library pwm.h
 * - pergerakan selain rotate akan mematikan perhitungan(startHeading =0)
 * - perhitungan heading terdapat pada fungsi getHeading yang akan menghitung perbedaan count
 * yang dicatat pada fungsi TIM2_IRQHandler di library speedcontrol.
 *
 */



void outBall1();
void outBall2();
void penalty();
void printData();
void init_proximity();
int getProxy();
void ballGet(void);
void startMain(int refree, int arahGawang);
void getBall(void);
void getGoalPos(void);
bool gotoHeadZero(void);
void getCoor(int headingProcess,int encoderDifference);
void tesRotateMap(void);
void LCD_InitializeTimer();
void LCD_EnableTimerInterrupt();
void TIM5_IRQHandler();
void init_lcdTimer();




#endif

