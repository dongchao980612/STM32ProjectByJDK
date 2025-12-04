#include "stm32f10x.h"                  // Device header
#include "delay.h"
#include "key.h"

// 按键配置
static KeyCfg_t g_keyCfgs0 =
{
    /* Key0 */
    GPIOE,
    RCC_APB2Periph_GPIOE,
    GPIO_Pin_4
};

static KeyCfg_t g_keyCfgs1 =
{
    /* Key1 */
    GPIOE,
    RCC_APB2Periph_GPIOE,
    GPIO_Pin_3
    // KEY1 通常上拉
};


const int SELECT_KEY = 0;

// 按键初始化
void Key0_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(g_keyCfgs0.keyClock, ENABLE);
    GPIO_InitStructure.GPIO_Pin  = g_keyCfgs0.keyPin;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;  // 上拉输入
    GPIO_Init(g_keyCfgs0.keyPort, &GPIO_InitStructure);
}

void Key1_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(g_keyCfgs1.keyClock, ENABLE);
    GPIO_InitStructure.GPIO_Pin  = g_keyCfgs1.keyPin;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;  // 上拉输入
    GPIO_Init(g_keyCfgs1.keyPort, &GPIO_InitStructure);
}

KeyMode_TypeDef Key0_GetStatus(void)
{
    if(GPIO_ReadInputDataBit(g_keyCfgs0.keyPort, g_keyCfgs0.keyPin) == KeyMode_PREDDED) //按下
    {
        Delay_ms(20);

        while(GPIO_ReadInputDataBit(g_keyCfgs0.keyPort, g_keyCfgs0.keyPin) == KeyMode_PREDDED)
        {
            Delay_ms(20);
            return KeyMode_PREDDED;
        }
    }

		
		return KeyMode_RELEASED;
}


KeyMode_TypeDef Key1_GetStatus(void){
	
	    if(GPIO_ReadInputDataBit(g_keyCfgs1.keyPort, g_keyCfgs1.keyPin) == KeyMode_PREDDED) //按下
    {
        Delay_ms(20);

        while(GPIO_ReadInputDataBit(g_keyCfgs1.keyPort, g_keyCfgs1.keyPin) == KeyMode_PREDDED)
        {
            Delay_ms(20);
            return KeyMode_PREDDED;
        }
    }
		
		return KeyMode_RELEASED;
}
