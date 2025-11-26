/**
  **********************************************************************************************
  * 简单循迹主程序示例
  * 展示如何使用 Track_Simple.c 进行循迹控制
  * 客户可以参考这个文件修改自己的主程序
  **********************************************************************************************
**/

#include "stm32f10x.h"
#include "PWM.h"
#include "Track_Simple.h"
#include "OLED.h"  // 如果有OLED显示屏

// 定时器初始化 - 10ms调用一次循迹控制
void Timer_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
	
	TIM_InternalClockConfig(TIM1);
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_Period = 10000 - 1;      // 10ms
	TIM_TimeBaseInitStructure.TIM_Prescaler = 72 - 1;      // 72MHz / 72 = 1MHz
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseInitStructure);
	
	TIM_ClearFlag(TIM1, TIM_FLAG_Update);
	TIM_ITConfig(TIM1, TIM_IT_Update, ENABLE);
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(&NVIC_InitStructure);
	
	TIM_Cmd(TIM1, ENABLE);
}

// 定时器中断服务函数 - 每10ms调用一次
void TIM1_UP_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM1, TIM_IT_Update) == SET)
	{
		// 调用循迹控制
		Simple_Track_Control();
		
		TIM_ClearITPendingBit(TIM1, TIM_IT_Update);
	}
}


int main(void)
{
	// ===== 1. 系统初始化 =====
	
	// 初始化PWM（电机控制）
	PWM_Init();
	
	// 初始化循迹传感器
	Simple_Track_Init();
	
	// 初始化OLED（可选，用于显示信息）
	// OLED_Init();
	// OLED_ShowString(1, 1, "Simple Track");
	
	// 初始化定时器
	Timer_Init();
	
	
	// ===== 2. 主循环 =====
	while(1)
	{
		// 定时器会自动调用循迹控制，这里可以做其他事情
		
		// 例如：显示传感器状态
		// OLED_ShowNum(2, 1, S1, 1);
		// OLED_ShowNum(2, 3, S2, 1);
		// ...
		
		// 或者什么都不做，等待定时器中断
	}
}


// ===== 如果不想用定时器，也可以用主循环方式 =====
/*
int main(void)
{
	PWM_Init();
	Simple_Track_Init();
	
	while(1)
	{
		// 直接在主循环中调用
		Simple_Track_Control();
		
		// 添加适当延时（约10ms）
		for(int i=0; i<72000; i++); // 简单延时
	}
}
*/


// ===== 手动测试示例 =====
/*
int main(void)
{
	PWM_Init();
	
	// 测试前进
	Go_Forward(4000);
	Delay_ms(2000);  // 前进2秒
	
	// 测试左转
	Turn_Left(3000);
	Delay_ms(1000);  // 左转1秒
	
	// 测试右转
	Turn_Right(3000);
	Delay_ms(1000);  // 右转1秒
	
	// 停止
	Stop_Motor();
	
	while(1);
}
*/
