#include "stm32f10x.h"                  // Device header
#include "delay.h"
#include "led.h"
#include "key.h"


int main(void)
{
  Led_Init();
  Key_Init();

  while (1)
    {
        // Led_On();
        // Delay_ms(1000);
        // Led_Off();
        // Delay_ms(1000);

      if(Key_GetNum() == 1)
        {
          Led_Turn();
        }

    }
}
