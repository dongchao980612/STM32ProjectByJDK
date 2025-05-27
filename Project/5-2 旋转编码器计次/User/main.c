#include "stm32f10x.h"                  // Device header
#include "oled.h"
#include "Encoder.h"

int16_t Num;
int main(void)
{
	// OLED初始化
	OLED_Init();		
	// 编码器初始化
	Encoder_Init();
	
	OLED_ShowString(1,1,"Num:");
	
	while (1)
    {
		Num+=Encoder_Get();
		OLED_ShowNum(1,5,Num,5);
    }
}
