#ifndef _TIMER_H_
#define _TIMER_H_

#include <reg52.h>

// ��������
void Timer0_Init(void);
void Timer0_Start(void);
void Timer0_Stop(void);

// �ⲿ��������
extern unsigned char hour, minute, second;
extern bit time_update;

#endif