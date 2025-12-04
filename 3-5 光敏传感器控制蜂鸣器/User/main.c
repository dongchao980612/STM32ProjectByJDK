#include "stm32f10x.h"                  // Device header
#include "delay.h"
#include "led.h"
#include "buzzer.h"
#include "lightsensor.h"


int main(void)
{
    Led0_Init();
    Buzzer_Init();
    LightSensor_Init();

    while (1)
    {

        if(LightSensor_GetStatus() == LIGHT_STATUS_BRIGHT)
        {
            Buzzer_Off();
            Led0_Off();
        }
        else
        {
            Buzzer_On();
            Led0_On();
        }
    }
}
