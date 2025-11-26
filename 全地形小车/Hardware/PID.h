#ifndef __PID_H
#define __PID_H

extern float RealValue[2],GoalValue[2];//≤‚¡ø÷µ

void TIM1_Init(void);
void PID(unsigned char Pid_Line);
void PID_x(void);
void PID_Switch(unsigned char NUM);
#endif
