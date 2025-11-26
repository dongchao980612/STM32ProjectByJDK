#include "stm32f10x.h"                  // Device header
#include "PWM.h" 
#include "PID.h" 
#include "OLED.h"
#include "Track.h"
#include "Delay.h"
#include "Encoder.h"

extern uint8_t Sensor_data;
extern uint8_t PID_Program;
extern float RealValue[2],GoalValue[2];//外部调用测量值
extern int Count;

void OLED_UP(void)
{
   OLED_ShowHexNum(2,1,Sensor_data,4); OLED_ShowHexNum(2,6,PID_Program,1);
   OLED_ShowNum(3,1,RealValue[0],4);  OLED_ShowNum(3,6,RealValue[1],4);
 	 OLED_ShowNum(4,1,Count,4);OLED_ShowNum(4,6,567,4);
}


extern int Count;

void main_Pro(void)
{
	while(1)
	{
			if(Sensor_data == 0X0F||Sensor_data == 0x07)
			{
				GoalValue[0] = 0;
				GoalValue[1] = 0;
				PID_Switch(2);
				Delay_ms(2000);
				GoalValue[0] = 7;
				GoalValue[1] = 7;
				PID_Switch(2);
				Delay_ms(2000);
			   GoalValue[0] = -15;    
            GoalValue[1] = 15;   
	  	   	PID_Switch(2);
	  	   	OLED_UP();
			}
			else if(Sensor_data == 0XF0||Sensor_data == 0xE0)
			{
				GoalValue[0] = 0;
				GoalValue[1] = 0;
				PID_Switch(2);
				Delay_ms(2000);
				GoalValue[0] = 7;
				GoalValue[1] = 7;
				PID_Switch(2);
				Delay_ms(2000);
			   GoalValue[0] = 15;    
            GoalValue[1] = -15;   
	  	   	PID_Switch(2);
	  	   	OLED_UP();
	  	   	Delay_ms(700);  
			}
			
			else if(Count == 5)
			{
			  Count  = 260;
			}
			else
			{
			  OLED_UP();
		     TrackData_Processing2();
		     PID_Program = 2;
			}

	}

}