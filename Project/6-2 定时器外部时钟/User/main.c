#include "stm32f10x.h"                  // Device header
#include "oled.h"
#include "timer.h"

int16_t Num;

int main(void)
{
	// OLED初始化
	OLED_Init();		

	Timer2_Init();
	
	OLED_ShowString(1,1,"Num:");
	OLED_ShowString(2,1,"CNT:");
	while (1)
    {
		OLED_ShowNum(1,5,Num,5);
		OLED_ShowNum(2,5,Timer2_GetCounter(),5);
    }
}

void TIM2_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM2,TIM_IT_Update)==SET)
	{
		Num++;
		TIM_ClearITPendingBit(TIM2,TIM_IT_Update);
	}	
}
