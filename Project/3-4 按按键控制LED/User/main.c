#include "stm32f10x.h"                  // Device header
#include "delay.h"
#include "led.h"
#include "key.h"

/**
  * 按键控制LED实验
  * 引脚：
  *      PB12
  * 现象：
  *      蜂鸣器开始鸣叫
  */
  
uint8_t keyNum = 0;
int main(void)
{
	LED_Init();
	Key_Init();
	while (1)
    {
//		LED1_ON();
//		Delay_ms(500);	
//		LED1_OFF();
//		Delay_ms(300);	
		keyNum = Key_GetNum();
		// PB8
		if(keyNum == 1){ 
			// LED1_ON();
			LED1_Trun();
		}
		// PB9
		if(keyNum == 2){
			// LED1_OFF();
			// LED1_Trun();
		}
    }
}
