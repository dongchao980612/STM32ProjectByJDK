#include "stm32f10x.h"                  // Device header
#include "delay.h"
#include "pwm.h"


int main(void)
{
    PWM_Init();



    while (1)
    {
        for (int i = 0; i <= 100; i++)
        {
            PWM_SetCompare2(i);
            Delay_ms(10);
        }

        for (int i = 0; i <= 100; i++)
        {
            PWM_SetCompare2(100 - i);
            Delay_ms(10);
        }

    }
}
