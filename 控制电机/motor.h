#ifndef __MOTOR_H__
#define __MOTOR_H__

#include <reg52.h>

// 引脚定义
sbit MOTOR = P1^0;     // 直流电机控制引脚
sbit KEY1 = P3^1;      // 独立按键K1

// 数码管位选控制
sbit DIG_WEI1 = P2^4;  // 最左侧数码管位选
sbit DIG_WEI2 = P2^3;
sbit DIG_WEI3 = P2^2;

// 数码管段选定义（共阴数码管）
#define DIG_SEG P0

// 档位定义
#define GEAR_0 0
#define GEAR_1 1
#define GEAR_2 2
#define GEAR_3 3
#define GEAR_4 4
#define GEAR_5 5

// 函数声明
void System_Init(void);
void Timer0_Init(void);
void Delay_ms(unsigned int ms);
void Display_Gear(unsigned char gear);
unsigned char Key_Scan(void);
void Motor_Control(unsigned char gear);

// 全局变量声明
extern unsigned char current_gear;
extern unsigned int timer0_count;

#endif