#include "main.h"
#include "gpio.h"

void GPIO_Configuration(void) {
  GPIO_InitTypeDef GPIO_InitStructure;
  
  // 使能GPIO时钟（例如循迹传感器引脚所在的GPIOA、GPIOB）
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB, ENABLE);

  // 循迹传感器引脚配置（输入模式）
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; // 例：OUT8 -> PB9
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; // 下拉输入
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  // 电机方向控制引脚配置（输出模式）
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13; // 电机1方向
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; // 推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
}