#ifndef __BUZZER_H
#define __BUZZER_H

typedef struct{
	/* Buzzer */
	GPIO_TypeDef*       buzzerPort;
	uint32_t            buzzerClock;
	uint16_t            buzzerPin;
} BuzzerCfg_t;


/**
  * @brief  初始化LED模块
  * @param  无
  * @retval 无
  */
void Buzzer_Init(void);

/**
  * @brief  蜂鸣器鸣叫
  * @param  无
  * @retval 无
  */
void Buzzer_Trun(void);

/**
  * @brief  打开蜂鸣器
  * @param  无
  * @retval 无
  */
void Buzzer_ON(void);

/**
  * @brief  关闭蜂鸣器
  * @param  无
  * @retval 无
  */
void Buzzer_OFF(void);

#endif
