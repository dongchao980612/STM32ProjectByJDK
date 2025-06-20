#include "stm32f10x.h"                  // Device header
#include "delay.h"
#include "led.h"
#include "key.h"
#include "buzzer.h"
#include "lightsensor.h"
  
uint8_t keyNum = 0;

int main(void)
{
	// LED_Init();
	// Key_Init();
	
	Buzzer_Init();
	LightSensor_Init();
	
	while (1)
    {
		if(LightSensor_GetNum() == 0)
			{
			Buzzer_ON();
			}
		else
			{
			Buzzer_OFF();
			}
    }
}
