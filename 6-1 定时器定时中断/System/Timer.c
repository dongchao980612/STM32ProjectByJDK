#include "stm32f10x.h"                  // Device header
#include "timer.h"

extern uint32_t  Number;

static timer2Cfg_t g_timer2Cfg =
{
    /* Timer2 */
    TIM2,
    RCC_APB1Periph_TIM2,

    TIM_CKD_DIV1,
    TIM_CounterMode_Up,

    10000 - 1,
    7200 - 1,
    0,


    /* NVIC */
    NVIC_PriorityGroup_2,
    TIM2_IRQn,
};


void Timer_Init(void)
{
    // 开启时钟
    RCC_APB1PeriphClockCmd(g_timer2Cfg.timerClock, ENABLE);

    // 配置内部时钟
    TIM_InternalClockConfig(g_timer2Cfg.timer);

    // 配置时基单元
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;

    //  时钟分频
    TIM_TimeBaseInitStructure.TIM_ClockDivision = g_timer2Cfg.timerClockDivision;
    // 计数模式
    TIM_TimeBaseInitStructure.TIM_CounterMode = g_timer2Cfg.timerCounterMode;
    // 周期
    TIM_TimeBaseInitStructure.TIM_Period = g_timer2Cfg.timerPeriod;
    TIM_TimeBaseInitStructure.TIM_Prescaler = g_timer2Cfg.timerPrescaler;
    // 重复计数器
    TIM_TimeBaseInitStructure.TIM_RepetitionCounter = g_timer2Cfg.timerRepetitionCounter;
    TIM_TimeBaseInit(g_timer2Cfg.timer, &TIM_TimeBaseInitStructure);

    TIM_ClearFlag(g_timer2Cfg.timer, TIM_FLAG_Update); 					// 清空更新中断标志位
    TIM_ITConfig(g_timer2Cfg.timer, TIM_IT_Update, ENABLE);


    /*NVIC中断分组*/
    NVIC_PriorityGroupConfig(g_timer2Cfg.nvicPriorityGroup);				//配置NVIC为分组2

    /*NVIC配置*/
    NVIC_InitTypeDef NVIC_InitStructure;																		//定义结构体变量
    NVIC_InitStructure.NVIC_IRQChannel = g_timer2Cfg.nvicIRQChannel;				//选择配置NVIC的TIM2_IRQn线
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;													//指定NVIC线路使能
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;								//指定NVIC线路的抢占优先级为1
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;											//指定NVIC线路的响应优先级为1
    NVIC_Init(&NVIC_InitStructure);


    //  启动定时器
    TIM_Cmd(g_timer2Cfg.timer, ENABLE);
}


uint16_t Timer2_GetCounter(void)
{
    return TIM_GetCounter(TIM2);
}


// 中断函数模板
void TIM2_IRQHandler(void)
{
    if(TIM_GetITStatus(g_timer2Cfg.timer, TIM_IT_Update) == SET)
    {
        Number++;
        TIM_ClearITPendingBit(g_timer2Cfg.timer, TIM_IT_Update);
    }
}
