#include "stm32f10x.h"                  // Device header
#include "delay.h"
#include "led.h"
#include "buzzer.h"
#include "lightsensor.h"


int main(void)
{
    led_init();
    buzzer_init();
    lightSensor_init();

    while (1)
    {

        if(lightSensor_getNum() == 0)
        {
            buzzer_on();
            led_off();
        }
        else
        {
            buzzer_off();
            led_on();
        }
    }
}
