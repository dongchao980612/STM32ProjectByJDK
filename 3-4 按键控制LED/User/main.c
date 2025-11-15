#include "stm32f10x.h"                  // Device header
#include "delay.h"
#include "led.h"
#include "key.h"


int main(void)
{
    led_init();
    key_init();

    while (1)
    {
//        led_on();
//        delay_ms(1000);
//        led_off();
//        delay_ms(1000);



        if(key_getNum() == 1)
        {
            led_turn();
        }

    }
}
