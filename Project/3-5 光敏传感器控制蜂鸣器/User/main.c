#include "stm32f10x.h"                  // Device header
#include "delay.h"
#include "led.h"
#include "key.h"
#include "buzzer.h"
#include "lightsensor.h"
/**
  * 光敏传感器控制蜂鸣器实验
  * 引脚：
  *      PB12
  * 现象：
  *      蜂鸣器开始鸣叫
  */
  
uint8_t keyNum = 0;
int main(void)
{
	// LED_Init();
	// Key_Init();
	Buzzer_Init();
	LightSensor_Init();
	while (1)
    {
		if(LightSensor_GetNum()==0){
			Buzzer_ON();
		}
		else{
			Buzzer_OFF();
		}
    }
}
