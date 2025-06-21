#ifndef __LED_H
#define __LED_H


typedef struct{
	/* Led */
	GPIO_TypeDef*       ledPort;
	uint32_t            ledClock;
	uint16_t            ledPin;
} LedCfg_t;


/**
  * @brief  初始化LED模块
  * @param  无
  * @retval 无
  */
void LED_Init(void);

/**
  * @brief  点亮LED
  * @param  无
  * @retval 无
  */
void LED_ON(void);

/**
  * @brief  关闭LED
  * @param  无
  * @retval 无
  */
void LED_OFF(void);

/**
  * @brief  LED反转
  * @param  无
  * @retval 无
  */
void LED_Trun(void);

#endif
