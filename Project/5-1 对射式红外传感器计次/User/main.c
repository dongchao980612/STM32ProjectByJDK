#include "stm32f10x.h"                  // Device header
#include "oled.h"
#include "CountSensor.h"

int main(void)
{
	/*模块初始化*/
	OLED_Init();		//OLED初始化
	CountSensor_Init();		//计数传感器初始化
	
	OLED_ShowString(1,1,"Count:");
	while (1)
    {
		OLED_ShowNum(1,7,CountSensor_Get(),5);
    }
}
