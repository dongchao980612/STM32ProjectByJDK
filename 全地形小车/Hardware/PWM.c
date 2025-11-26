/**
  **********************************************************************************************
  *本代码为电机驱动程序
  *包含PWM定时器初始化&电机启驱动
  *PWM部分：应用TIM2的通道1和通道2输出两路PWM波
            通道1对应的引脚为PA0
			   通道2对应的引脚为PA1
  *电机驱动部分：初始化PA4 PA5 PB0 PB1
                 电机1：PA4 PA5
					  电机2：PB0 PB1
  *Written by Mr. Wangrui
  *[ November 11th, 2025]
  **********************************************************************************************
**/

/* --------------------------------MAIN TEST CODE----------------------------------------*/
#include "stm32f10x.h"                  // Device header
/**
  * PWM定时器初始化函数
  * 使用定时器2
 **/
void PWM_Init(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA , ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	TIM_InternalClockConfig(TIM2);
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_Period = 10000 - 1;		//ARR
	TIM_TimeBaseInitStructure.TIM_Prescaler = 72 - 1;		//PSC
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStructure);
	
	TIM_OCInitTypeDef TIM_OCInitStructure;
	TIM_OCStructInit(&TIM_OCInitStructure);
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = 0;		//CCR
	TIM_OC1Init(TIM2, &TIM_OCInitStructure);
	TIM_OC2Init(TIM2 ,&TIM_OCInitStructure);
	TIM_Cmd(TIM2, ENABLE);
	
	//电机初始化
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_4 | GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_0 | GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
}

/**
  * 电机驱动函数
  * 调用此函数可以设置某一个电机的旋转方向及PWM
  * 由于PWM定时器初始化时ARR= 10000 - 1 PSC= 72 - 1 所以导致当PWM设置大约为3000的时候电机会旋转
  * PWM_Line：此变量用于设定1号电机或2号电机 可写入值为 1或2
  * PWM_Data：此变量用于设定PWM的值
 **/
void PWM_Motor_Set(unsigned char PWM_Line,int PWM_Data)
{
	switch(PWM_Line)
	{
		case 1://左侧电机控制
			if(PWM_Data >= 0)
			{
				GPIO_SetBits(GPIOB,GPIO_Pin_0);
				GPIO_ResetBits(GPIOB,GPIO_Pin_1);//正转
				TIM_SetCompare1(TIM2,PWM_Data);
			}
			else
			{
				GPIO_SetBits(GPIOB,GPIO_Pin_1);
				GPIO_ResetBits(GPIOB,GPIO_Pin_0);//反转	
				TIM_SetCompare1(TIM2, -PWM_Data);
			}
			break;
			
		case 2://右侧电机控制
			if(PWM_Data >= 0)
			{
				GPIO_SetBits(GPIOA,GPIO_Pin_5);
				GPIO_ResetBits(GPIOA,GPIO_Pin_4);//正转
				TIM_SetCompare2(TIM2,PWM_Data);
			}
			else
			{
				GPIO_SetBits(GPIOA,GPIO_Pin_4);
				GPIO_ResetBits(GPIOA,GPIO_Pin_5);//反转
				TIM_SetCompare2(TIM2,-PWM_Data);
			}
			break;
	}
	
}
	

