#include "stm32f10x.h"                  // Device header
#include "oled.h"
#include "timer.h"

int16_t Num = 0;

int main(void)
{
	// OLED初始化
	OLED_Init();		
	Timer2_Init();
	
	while (1)
    {
		OLED_ShowNum(1,5,Num,5);
		OLED_ShowNum(2,5,TIM_GetCounter(TIM2),5); // 查看计数器值的变化
    }
}

void TIM2_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM2,TIM_IT_Update) == SET)
	{
		Num++;
		TIM_ClearITPendingBit(TIM2,TIM_IT_Update);
	}	
}
