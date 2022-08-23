#ifndef SPEED_CONTROL_H_
#define SPEED_CONTROL_H_
#include <stdint.h>

void InitializeTimer();
void EnableTimerInterrupt();
void TIM2_IRQHandler();
void TIM5_IRQHandler();
void init_speed_control(void);
void motorSpeed(int16_t leftBackSpeed, int16_t leftFrontSpeed, int16_t righFrontSpeed, int rightBackSpeed);

void maju(int32_t pwm);
void mundur(int32_t pwm);
void kanan(int32_t pwm);
void kiri(int32_t pwm);
void timurLaut(int32_t pwm);
void tenggara(int32_t pwm);
void baratLaut(int32_t pwm);
void baratDaya(int32_t pwm);
void rotateClockWise(int32_t pwm);
void rotateAntiClockWise(int32_t pwm);

#endif //SPEED_CONTROL_H_
