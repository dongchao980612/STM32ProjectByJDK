/**
  **********************************************************************************************
  *本代码为编码器初始化程序
  *包含编码器定时器初始化函数 获取编码器数值函数
  *本程序所用定时器为TIM3 TIM4
  *硬件连接：1号电机：PA6 PA7
             2号电机：PB6 PB7
  *Written by Mr. Wangrui
  *[ November 11th, 2025]
  **********************************************************************************************
**/

/* --------------------------------MAIN TEST CODE----------------------------------------*/
#include "stm32f10x.h"                  // Device header

/**
  * @brief  编码器初始化函数
  * @param  无
  * @retval 无
 **/
void Encoder_Init(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_Period = 65536 - 1;		//ARR
	TIM_TimeBaseInitStructure.TIM_Prescaler = 1 - 1;		//PSC
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStructure);
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseInitStructure);
	
	TIM_ICInitTypeDef TIM_ICInitStructure;
	TIM_ICStructInit(&TIM_ICInitStructure);
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;
	TIM_ICInitStructure.TIM_ICFilter = 0xF;
	TIM_ICInit(TIM3, &TIM_ICInitStructure);
	TIM_ICInit(TIM4, &TIM_ICInitStructure);
	
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_2;
	TIM_ICInitStructure.TIM_ICFilter = 0xF;
	TIM_ICInit(TIM3, &TIM_ICInitStructure);
	TIM_ICInit(TIM4, &TIM_ICInitStructure);
	
	TIM_EncoderInterfaceConfig(TIM3, TIM_EncoderMode_TI12, TIM_ICPolarity_Falling, TIM_ICPolarity_Falling);
	TIM_EncoderInterfaceConfig(TIM4, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Falling);

	TIM_Cmd(TIM3, ENABLE);
	TIM_Cmd(TIM4, ENABLE);
	
	// 设置编码器初始值，避免第一次读取异常
	TIM_SetCounter(TIM3, 1000);
	TIM_SetCounter(TIM4, 1000);
}

extern float RealValue[2],GoalValue[2];//外部调用测量值
extern uint8_t PID_Program;
/**
  * 读取编码器数值函数
  * 调用此函数可以获取到编码器所测量到的数值
 **/
void Encoder_Get(void)
{
		RealValue[0]=TIM_GetCounter(TIM3)-1000;
		TIM_SetCounter(TIM3,1000);
		
		RealValue[1]=TIM_GetCounter(TIM4)-1000;
		TIM_SetCounter(TIM4,1000);
}
/**
  * 编码器清零函数
  * 调用此函数可清零编码器
 **/
void Encoder_Clear(void)
{
			TIM_SetCounter(TIM3,1000);
	      TIM_SetCounter(TIM4,1000);
}

