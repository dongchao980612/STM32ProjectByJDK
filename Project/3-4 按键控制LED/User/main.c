#include "stm32f10x.h"                  // Device header
#include "delay.h"
#include "led.h"
#include "key.h"
  
uint8_t keyNum = 0;

int main(void)
{
	LED_Init();
	Key_Init();
	
	while (1)
    {
//		LED_ON();
//		Delay_ms(500);	
//		LED_OFF();
//		Delay_ms(300);	
			
		keyNum = Key_GetNum();
		// PC14
		if(keyNum == 1)
			{ 
			// LED_ON();
			LED_Trun();
			}
    }
}
