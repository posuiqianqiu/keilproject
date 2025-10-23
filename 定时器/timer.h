#ifndef TIMER_H
#define TIMER_H

#include <reg52.h>

// 定时器相关定义
#define FOSC 12000000UL  // 晶振频率12MHz
#define T1MS (65536 - FOSC/12/1000)  // 1ms定时初值

// 函数声明
void Timer0_Init(void);
void Timer0_SetInterrupt(void (*func)(void));

#endif