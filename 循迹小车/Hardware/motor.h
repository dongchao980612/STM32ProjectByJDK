#ifndef __MOTOR_H
#define  __MOTOR_H

void TIM_PWM_Configuration(void);

// 设置电机PWM（方向+占空比）
void motor_set_pwm(int motor1, int motor2);

#endif