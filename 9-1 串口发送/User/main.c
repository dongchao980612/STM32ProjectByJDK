#include "stm32f10x.h"                  // Device header
#include "usart.h"
#include "delay.h"

int main(void)
{
    My_USART_Init();

    while (1)
    {
        My_Printf("Hello STM32!\n");
        Delay_s(1);
    }
}
