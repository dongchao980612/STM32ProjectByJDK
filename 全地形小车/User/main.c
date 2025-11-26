#include "stm32f10x.h"                  // Device header
#include "PWM.h" 
#include "OLED.h"
#include "Track.h"
#include "Encoder.h"
#include "PID.h"
#include "Delay.h"

extern float RealValue[2], GoalValue[2];
extern uint8_t Sensor_data;
extern uint8_t PID_Program;

int main(void)
{ 
	PWM_Init();
	OLED_Init();
	Encoder_Init();
	Tracking_Init();
	
	// 延时让硬件稳定，再启动PID定时器
	Delay_ms(100);
	
	// 最后启动PID定时器中断，此时所有外设已就绪
	TIM1_Init();
	
	Delay_ms(500);
	
	while(1)
	{ 
		OLED_ShowString(1, 1, "PID Track");
		OLED_ShowString(2, 1, "Sen:");
		OLED_ShowHexNum(2, 5, Sensor_data, 2);
		OLED_ShowString(3, 1, "L:");
		OLED_ShowSignedNum(3, 3, (int)RealValue[0], 4);
		OLED_ShowString(4, 1, "R:");
		OLED_ShowSignedNum(4, 3, (int)RealValue[1], 4);
	}
}

// 定时器中断服务函数
void TIM1_UP_IRQHandler(void)
{  
	if(TIM_GetITStatus(TIM1, TIM_IT_Update) == SET)
	{
		Encoder_Get();
		TrackData_Processing();
		PID_x();
		PID(1);
		PID(2);
		TIM_ClearITPendingBit(TIM1, TIM_IT_Update);
	}
}