// motor.c
#include "main.h"
#include "motor.h"

void TIM_PWM_Configuration(void) {
  TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
  TIM_OCInitTypeDef TIM_OCInitStructure;

  // 使能TIM1时钟（电机PWM输出）
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);

  // 时基配置（例如8000分频，对应PWM频率）
  TIM_TimeBaseStructure.TIM_Period = 7999; // 周期=8000-1
  TIM_TimeBaseStructure.TIM_Prescaler = 0;
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);

  // PWM模式配置（TIM1_CH1和CH2对应左右电机）
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = 0; // 初始占空比0
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
  TIM_OC1Init(TIM1, &TIM_OCInitStructure); // 电机1
  TIM_OC2Init(TIM1, &TIM_OCInitStructure); // 电机2

  TIM_Cmd(TIM1, ENABLE); // 使能定时器
  TIM_CtrlPWMOutputs(TIM1, ENABLE); // 使能PWM输出（高级定时器需此步）
}

// 设置电机PWM（方向+占空比）
void motor_set_pwm(int motor1, int motor2) {
  // 电机1方向控制（例：PB12、PB13）
  if (motor1 > 0) {
    GPIO_SetBits(GPIOB, GPIO_Pin_12);
    GPIO_ResetBits(GPIOB, GPIO_Pin_13);
  } else {
    GPIO_ResetBits(GPIOB, GPIO_Pin_12);
    GPIO_SetBits(GPIOB, GPIO_Pin_13);
  }
  TIM_SetCompare1(TIM1, motor1); // 设置占空比

  // 电机2方向控制（例：PB14、PB15）
  if (motor2 > 0) {
    GPIO_SetBits(GPIOB, GPIO_Pin_14);
    GPIO_ResetBits(GPIOB, GPIO_Pin_15);
  } else {
    GPIO_ResetBits(GPIOB, GPIO_Pin_14);
    GPIO_SetBits(GPIOB, GPIO_Pin_15);
  }
  TIM_SetCompare2(TIM1,motor2);
}
