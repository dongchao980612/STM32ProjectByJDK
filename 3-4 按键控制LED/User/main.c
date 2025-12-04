#include "stm32f10x.h"                  // Device header
#include "delay.h"
#include "led.h"
#include "key.h"


int main(void)
{
    Led0_Init();
    Led1_Init();

    Key0_Init();
    Key1_Init();

    // Led0_On();
    // Led1_On();


    while (1)
    {
        // Led0_On();
        // Delay_ms(1000);
        // Led0_Off();
        // Delay_ms(1000);
        #if  1
        if(Key0_GetStatus() == KeyMode_PREDDED)
        {
            Led0_On();
        }
        else
        {
            Led0_Off();
        }



        if(Key1_GetStatus() == KeyMode_PREDDED)
        {

            Led1_On();
        }
        else
        {

            Led1_Off();
        }


        #endif

    }
}
