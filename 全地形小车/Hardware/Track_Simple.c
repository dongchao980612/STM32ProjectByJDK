/**
  **********************************************************************************************
  * 简单循迹控制 - 无PID版本
  * 直接通过传感器状态控制电机速度
  * 客户可以根据实际情况调整速度值
  * Written by AI Assistant
  * [November 17th, 2025]
  **********************************************************************************************
**/

#include "stm32f10x.h"
#include "PWM.h"
#include "OLED.h"
#include "Encoder.h"

// ========== 可调参数区域 - 客户可以修改这些值 ==========
#define SPEED_STRAIGHT  4000   // 直行速度
#define SPEED_TURN_FAST 3000   // 急转弯速度
#define SPEED_TURN_SLOW 2000   // 缓转弯速度
#define SPEED_STOP      0      // 停止

// 左右轮速度补偿 - 如果小车直行时偏向一边，调整这两个值
// 根据编码器数据：左轮17，右轮13，左轮快约30%，需要让左轮减速、右轮加速
#define LEFT_WHEEL_OFFSET   -600   // 左轮补偿（负值=减速，让快的左轮慢下来）
#define RIGHT_WHEEL_OFFSET  600    // 右轮补偿（正值=加速，让慢的右轮加速）
// 注意：这两个值需要根据实际测试微调，目标是让两轮编码器数值接近

// 传感器引脚定义 - 从右到左依次为 S1 S2 S3 S4 S5 S6 S7 S8
#define S1  GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_4)   // 最右
#define S2  GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_5)
#define S3  GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_10)
#define S4  GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_11)
#define S5  GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_12)
#define S6  GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_13)
#define S7  GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_14)
#define S8  GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_15)  // 最左

// 黑线定义：0=检测到黑线  1=白色（无线）
// ========================================================


/**
  * 简单循迹初始化
 **/
void Simple_Track_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; // 上拉输入
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_10 | GPIO_Pin_11 |
	                               GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
}


/**
  * 设置左右轮速度
  * left_speed: 左轮速度 (-10000 到 10000)
  * right_speed: 右轮速度 (-10000 到 10000)
 **/
void Set_Motor_Speed(int left_speed, int right_speed)
{
	// 应用速度补偿
	left_speed += LEFT_WHEEL_OFFSET;
	right_speed += RIGHT_WHEEL_OFFSET;
	
	// 限幅保护
	if (left_speed > 10000) left_speed = 10000;
	if (left_speed < -10000) left_speed = -10000;
	if (right_speed > 10000) right_speed = 10000;
	if (right_speed < -10000) right_speed = -10000;
	
	PWM_Motor_Set(1, left_speed);   // 左轮
	PWM_Motor_Set(2, right_speed);  // 右轮
}


/**
  * 简单循迹主控制函数
  * 直接根据传感器状态控制电机
 **/
void Simple_Track_Control(void)
{
	int left_speed = 0;
	int right_speed = 0;
	
	// ====== 1. 直行状态 - 中间传感器检测到线 ======
	if ((S4 == 0 && S5 == 0) || (S3 == 0 && S4 == 0 && S5 == 0 && S6 == 0))
	{
		// 中间有线，直行
		left_speed = SPEED_STRAIGHT;
		right_speed = SPEED_STRAIGHT;
	}
	
	// ====== 2. 轻微偏右 - 需要向左微调 ======
	else if (S3 == 0 || (S3 == 0 && S4 == 0))
	{
		// 右侧传感器检测到线，左轮加速，右轮减速
		left_speed = SPEED_STRAIGHT;
		right_speed = SPEED_TURN_SLOW;
	}
	
	// ====== 3. 严重偏右 - 需要向左急转 ======
	else if (S1 == 0 || S2 == 0 || (S1 == 0 && S2 == 0) || (S2 == 0 && S3 == 0))
	{
		// 最右侧传感器检测到线，急转
		left_speed = SPEED_TURN_FAST;
		right_speed = -SPEED_TURN_SLOW;  // 右轮倒转，增强转向
	}
	
	// ====== 4. 轻微偏左 - 需要向右微调 ======
	else if (S6 == 0 || (S5 == 0 && S6 == 0))
	{
		// 左侧传感器检测到线，右轮加速，左轮减速
		left_speed = SPEED_TURN_SLOW;
		right_speed = SPEED_STRAIGHT;
	}
	
	// ====== 5. 严重偏左 - 需要向右急转 ======
	else if (S7 == 0 || S8 == 0 || (S7 == 0 && S8 == 0) || (S6 == 0 && S7 == 0))
	{
		// 最左侧传感器检测到线，急转
		left_speed = -SPEED_TURN_SLOW;   // 左轮倒转，增强转向
		right_speed = SPEED_TURN_FAST;
	}
	
	// ====== 6. 全白 - 丢线情况 ======
	else if (S1 && S2 && S3 && S4 && S5 && S6 && S7 && S8)
	{
		// 所有传感器都是白色，可能丢线
		// 保持原速度继续前进或停止
		left_speed = SPEED_STRAIGHT / 2;
		right_speed = SPEED_STRAIGHT / 2;
	}
	
	// ====== 7. 全黑 - 特殊路口或终点 ======
	else if (S1 == 0 && S2 == 0 && S3 == 0 && S4 == 0 && 
	         S5 == 0 && S6 == 0 && S7 == 0 && S8 == 0)
	{
		// 所有传感器都检测到黑线
		// 可能是路口、起点或终点，这里选择直行
		left_speed = SPEED_STRAIGHT;
		right_speed = SPEED_STRAIGHT;
	}
	
	// ====== 8. 其他情况 - 默认直行 ======
	else
	{
		left_speed = SPEED_STRAIGHT;
		right_speed = SPEED_STRAIGHT;
	}
	
	// 设置电机速度
	Set_Motor_Speed(left_speed, right_speed);
}


/**
  * 手动控制函数 - 客户可以用这些测试电机
 **/
 
// 直行
void Go_Forward(int speed)
{
	Set_Motor_Speed(speed, speed);
}

// 后退
void Go_Backward(int speed)
{
	Set_Motor_Speed(-speed, -speed);
}

// 左转（原地）
void Turn_Left(int speed)
{
	Set_Motor_Speed(-speed, speed);
}

// 右转（原地）
void Turn_Right(int speed)
{
	Set_Motor_Speed(speed, -speed);
}

// 停止
void Stop_Motor(void)
{
	Set_Motor_Speed(0, 0);
}


/**
  * OLED显示函数 - 显示调试信息
  * 显示内容：
  *   第1行：标题
  *   第2行：传感器状态（十六进制）
  *   第3行：左轮编码器数值
  *   第4行：右轮编码器数值
 **/
extern float RealValue[2];  // 从Encoder.c获取的编码器数值

// 读取传感器状态（返回8位数据）
uint8_t Get_Sensor_Status(void)
{
	uint8_t sensor_data = 0;
	sensor_data |= GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_4)  << 0;
	sensor_data |= GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_5)  << 1;
	sensor_data |= GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_10) << 2;
	sensor_data |= GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_11) << 3;
	sensor_data |= GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_12) << 4;
	sensor_data |= GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_13) << 5;
	sensor_data |= GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_14) << 6;
	sensor_data |= GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_15) << 7;
	return sensor_data;
}

void Simple_OLED_Display(void)
{
	uint8_t sensor_status = Get_Sensor_Status();
	
	// 第1行：标题
	OLED_ShowString(1, 1, "Track Simple");
	
	// 第2行：传感器状态（十六进制，0=黑线 1=白色）
	OLED_ShowString(2, 1, "Sen:");
	OLED_ShowHexNum(2, 5, sensor_status, 2);
	
	// 第3行：左轮编码器数值
	OLED_ShowString(3, 1, "L:");
	OLED_ShowSignedNum(3, 3, (int)RealValue[0], 4);
	
	// 第4行：右轮编码器数值
	OLED_ShowString(4, 1, "R:");
	OLED_ShowSignedNum(4, 3, (int)RealValue[1], 4);
}
