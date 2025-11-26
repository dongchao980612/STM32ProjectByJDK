/**
  **********************************************************************************************
  *本代码为循迹模块代码
  *包含初始化程序 循迹模块数值获取程序 偏移距离数据处理程序
  *本代码为四路循迹 所用引脚为 PB4 PB5 PB10 PB11 PB12 PB13 PB14 PB15 
  *Written by Mr. Wangrui
  *[ November 11th, 2025]
  **********************************************************************************************
**/

/* --------------------------------MAIN TEST CODE----------------------------------------*/
#include "stm32f10x.h"                  // Device header

uint8_t Sensor_data = 0;
uint8_t PID_Program = 0;

extern float RealValue[2],GoalValue[2];//外部调用测量值
/**
  * 循迹模块初始化函数
 **/
void Tracking_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //上拉输入模式确保传感器输出信号在没有检测到黑线时保持稳定的高电平状态
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_10 | GPIO_Pin_11 |GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
}

/**
  * 读取循迹模块数值并处理
 **/

uint8_t Read_Sensors(void)
{
	uint8_t sensor_data = 0;  //初始化一个八位变量储存数据
	sensor_data |= GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_4)  << 0;
	sensor_data |= GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_5)  << 1;
	sensor_data |= GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_10) << 2;
	sensor_data |= GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_11) << 3;
	sensor_data |= GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_12) << 4;
	sensor_data |= GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_13) << 5;
	sensor_data |= GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_14) << 6;
	sensor_data |= GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_15) << 7;
	//读取传感器状态来判断小车是否偏移
	return sensor_data;
}

int Track = 0;//偏移距离
int Last_Track = 0;//记录上一次的偏差方向

/**
  * 数据处理
 **/

void TrackData_Processing(void)
{

	Sensor_data = Read_Sensors();
	switch(Sensor_data)
		{
			//直行
			case 0xE7:
			      Track = 0;
			      break;
			case 0xC3:
			      Track = 0;
			      break;
			case 0xC7:
			      Track = -1;
			      break;
			case 0xE3:
			      Track = 1;
			      break;
			
			//偏右（重要：补全缺失的右偏情况）
			case 0x01:  //最右边1个
			      Track = -5;
			      break;
			case 0x03:  //最右边2个
			      Track = -5;
			      break;
			case 0x07:  //最右边3个（之前缺失！）
			      Track = -5;
			      break;
			case 0x0F:  //最右边4个
			      Track = -5;
			      break;
			case 0x7F:
			      Track = -5;
			      break;
			case 0x3F:
			      Track = -4;
			      break;
			case 0x1F:
			      Track = -3;
			      break;
			case 0x9F:
			      Track = -3;
			      break;
			case 0xCF:
			      Track = -2;
			      break;
			
			//偏左
			case 0x80:  //最左边1个
			      Track = 5;
			      break;
			case 0xC0:  //最左边2个
			      Track = 5;
			      break;
			case 0xE0:  //最左边3个
			      Track = 5;
			      break;
			case 0xF0:  //最左边4个
			      Track = 5;
			      break;
			case 0xFE:
			      Track = 5;
			      break;
			case 0xFC:
			      Track = 4;
			      break;
			case 0xF8:
			      Track = 3;
			      break;
			case 0xF9:
			      Track = 3;
			      break;
			case 0xF1:
			      Track = 2;
			      break;
			case 0xF3:
			      Track = 2;
			      break;
			//全白（丢线）：保持上次方向继续转
		   case 0xFF:
			      Track = Last_Track; //保持上一次的偏差，继续转弯
			      break;
			//全黑（可能在起点）
		   case 0x00:
			      Track = 0;
			      break;
			//其他未定义情况：保持上次方向
		   default:
			      Track = Last_Track;
			      break;
		}
	
	//记录本次偏差（排除直线情况）
	if(Track != 0)
	{
		Last_Track = Track;
	}
	
	
}

void Forward(uint16_t F)
{
	GoalValue[0] = F;
   GoalValue[1] = F; 
	PID_Program = 2; 
	
}

void left(uint16_t L)
{
	GoalValue[0] = -L;
   GoalValue[1] = L; 
	PID_Program = 2; 
	
}

void Right(uint16_t R)
{
	
	GoalValue[0] = R;
   GoalValue[1] = -R; 
	PID_Program = 2; 
	
	
}

int Count = 0;

void TrackData_Processing2(void)
{

	Sensor_data = Read_Sensors();
	switch(Sensor_data)
		{
			//直行
			case 0xE7:
			      Forward(12);
			      break;
			case 0xC3:
			       Forward(12);
			      break;
			case 0xC7:
			      Forward(12);
			      break;
			case 0xE3:
			       Forward(12);
			      break;
			case 0x00:
			       Forward(12);
			       Count++;
			      break;
			//偏右
			
			case 0x7F:
			       GoalValue[0] = 16;
            GoalValue[1] = 12; 
	          PID_Program = 2; 
			      break;
			case 0x3F:
			       GoalValue[0] = 15;
            GoalValue[1] =12; 
	          PID_Program = 2; 
			      break;
			      break;
			case 0x1F:
			      Right(4);
			      break;
			case 0x9F:
			      GoalValue[0] = 15;
            GoalValue[1] = 12; 
	          PID_Program = 2; 
			      break;
			      break;
			case 0xCF:
			      GoalValue[0] = 15;
            GoalValue[1] = 12; 
	          PID_Program = 2; 
			      break;
			      break;
			
			//偏左
			case 0xFE:
			      GoalValue[0] = 12;
            GoalValue[1] = 16; 
	          PID_Program = 2; 
			      break;
			case 0xFC:
			      GoalValue[0] = 12;
            GoalValue[1] = 15; 
	          PID_Program = 2; 
			      break;
			case 0xF8:
			     GoalValue[0] =12;
            GoalValue[1] = 15; 
	          PID_Program = 2; 
			      break;
			case 0xF9:
			     GoalValue[0] = 12;
            GoalValue[1] = 15; 
	          PID_Program = 2; 
			      break;
			case 0xF1:
			     GoalValue[0] = 12;
            GoalValue[1] = 15; 
	          PID_Program = 2; 
			      break;
			case 0xF3:
			      GoalValue[0] = 12;
            GoalValue[1] = 15; 
	          PID_Program = 2; 
			      break;
			
			
			//全白
//		   case 0xFF:
//			      Forward(0);
//			      break;	
		}
	
	
}