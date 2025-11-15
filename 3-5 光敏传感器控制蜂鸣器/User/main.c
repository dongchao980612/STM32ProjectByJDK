#include "stm32f10x.h"                  // Device header
#include "delay.h"
#include "led.h"
#include "buzzer.h"
#include "lightsensor.h"


int main(void)
{
    Led_Init();
    Buzzer_Init();
    LightSensor_Init();

    while (1)
    {

        if(LightSensor_GetNum() == 0)
        {
            Buzzer_On();
            Led_Off();
        }
        else
        {
            Buzzer_Off();
            Led_On();
        }
    }
}
