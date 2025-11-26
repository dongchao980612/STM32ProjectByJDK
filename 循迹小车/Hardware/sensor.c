// track_control.c
#include "main.h"
#include "control.h"
#include "sensor.h"
#include "motor.h"

uint8_t track_read(void) {
  uint8_t val[8];
  // 读取8路循迹传感器（标准库GPIO读取函数）
  val[0] = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_9); // OUT8
  val[1] = GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_12); // OUT7
  // ... 其他6路传感器（略）
  // 组合为8位数据（同原逻辑）
  uint8_t val1 = 0;
  val1 |= (val[7] << 7) | (val[6] << 6)  | (val[0] << 0);
  return val1;
}

void track_task(void) {
  uint8_t count = track_read();
  switch (count) {
    case 2: // 00000010，左偏
      motor_set_pwm(4600 - 22*40, 4600 + 22*40); // 对应原turn(error)逻辑
      break;
    // ... 其他循迹状态（同原逻辑）
    case 255: // 全高电平，停止
      motor_set_pwm(0, 0);
      break;
  }
}