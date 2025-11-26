// main.c 初始化部分
#include "main.h"
#include "gpio.h"
#include "uart.h"
#include "timer.h"
#include "motor.h"

int main(void) {
  // 系统时钟初始化（标准库函数）
  SystemInit();           // 初始化系统时钟（来自system_stm32f10x.c）
  SysTick_Configuration();// 配置滴答定时器（如需延时）

  // 外设初始化
  GPIO_Configuration();   // GPIO初始化（BSP层）
  USART_Configuration();  // 串口初始化（JY61P用USART2）
  TIM_PWM_Configuration();// 定时器PWM初始化（电机驱动）

  // 使能串口中断（接收JY61P数据）
  USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);

  while (1) {
    // 主循环逻辑（循迹、控制等）
    track_task();
  }
}