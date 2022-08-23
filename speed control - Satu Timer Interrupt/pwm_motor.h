#ifndef PWM_MOTOR_H_
#define PWM_MOTOR_H_

void TM_TIMER_Init();
void init_pwm();
void init_motor();
void motorDC(uint8_t channel, int32_t pwm);

#endif
