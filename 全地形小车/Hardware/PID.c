/**
  **********************************************************************************************
  *本代码为串环PID代码
  *包含定时器初始化程序 速度环 位置环
  *Written by Mr. Wangrui
  *[ November 11th, 2025]
  **********************************************************************************************
**/

/* --------------------------------MAIN TEST CODE----------------------------------------*/
#include "stm32f10x.h"                  // Device header
#include "Encoder.h"
#include "PWM.h"
#include "PID.h"
#include "OLED.h"
extern uint8_t Sensor_data;
extern uint8_t PID_Program;

//限幅函数
void Ixianfu(int* IError,int xianfu_Value)
{
	if(*IError>xianfu_Value)*IError=xianfu_Value;
	if(*IError<-xianfu_Value)*IError=-xianfu_Value;
}
/**
  * 定时器初始化函数
  * 使用定时器1
  * 定时中断时间为5ms一次
 **/
void TIM1_Init(void) //5ms
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
	
	TIM_InternalClockConfig(TIM1);
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_Period = 5000 - 1;
	TIM_TimeBaseInitStructure.TIM_Prescaler = 72 - 1;
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
	OLED_ShowString(1,1,"WRV1.0");
}


/*	PI速度环	*/
float P[2]={300,300};//比例系数
float I[2]={9,9};//积分系数
int NowError[2];//误差值
float RealValue[2],GoalValue[2]={0,0};//测量值，目标值
int IError[2];//累计积分值
int PWM_Set[2];//pwm占空比(1/10000)

// 速度补偿（根据实际测试调整）
float Left_Offset = -3.0;
float Right_Offset = -3.0;

void PID(unsigned char Pid_Line)
{
	NowError[Pid_Line-1]=GoalValue[Pid_Line-1]-RealValue[Pid_Line-1];//计算误差  当前误差 = 目标值 - 测量值
	IError[Pid_Line-1]+=NowError[Pid_Line-1];//误差积分
	Ixianfu(&IError[Pid_Line-1],3000);//积分限幅
	if(GoalValue[Pid_Line-1]==0)IError[Pid_Line-1]=0;//消除残留积分
	
	PWM_Set[Pid_Line-1]= P[Pid_Line-1]*NowError[Pid_Line-1]+I[Pid_Line-1]*IError[Pid_Line-1];
	if(PWM_Set[Pid_Line-1]>10000)PWM_Set[Pid_Line-1]=10000;
	if(PWM_Set[Pid_Line-1]<-10000)PWM_Set[Pid_Line-1]=-10000;
	
	// 添加速度补偿到PWM输出（而不是目标值）
	if(Pid_Line == 1) PWM_Set[Pid_Line-1] += (int)(Left_Offset * 100);  // 左轮补偿
	if(Pid_Line == 2) PWM_Set[Pid_Line-1] += (int)(Right_Offset * 100); // 右轮补偿
	
	PWM_Motor_Set(Pid_Line,PWM_Set[Pid_Line-1]);
}


/*循迹PD位置环*/
float P_x = 2.0;//比例系数（90度急弯专用：超强转向力度）
float D_x = 2.5;//微分系数（减小阻尼，允许快速转向）
extern int Track;//偏移量
float LastError_x;//上一刻误差
float DError_x;//微分值
int Set_v = 15;//设定速度

void PID_x(void)
{
	DError_x = LastError_x-Track;//计算微分值
	
	// 目标值不加补偿，保持两边一致
	GoalValue[0] = Set_v-(P_x*Track-D_x*DError_x);  //左轮目标
	GoalValue[1] = Set_v+(P_x*Track-D_x*DError_x);  //右轮目标
	
	LastError_x = Track;//记录上一刻误差
}

void PID_Switch(unsigned char NUM)
{
	TIM_SetCounter(TIM3,1000);
	TIM_SetCounter(TIM4,1000);
	PID_Program = NUM;
}

