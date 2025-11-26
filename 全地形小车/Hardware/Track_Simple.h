#ifndef __TRACK_SIMPLE_H
#define __TRACK_SIMPLE_H

/**
  * 简单循迹控制 - 头文件
  * 客户可以在main函数中调用这些函数
 **/

// 初始化函数
void Simple_Track_Init(void);

// 主循迹控制函数（在定时器中断或主循环中调用）
void Simple_Track_Control(void);

// 手动控制函数
void Set_Motor_Speed(int left_speed, int right_speed);
void Go_Forward(int speed);
void Go_Backward(int speed);
void Turn_Left(int speed);
void Turn_Right(int speed);
void Stop_Motor(void);

// OLED显示函数（显示传感器和编码器数据）
void Simple_OLED_Display(void);

#endif
