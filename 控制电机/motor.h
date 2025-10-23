#ifndef __MOTOR_H__
#define __MOTOR_H__

#include <reg52.h>

// ���Ŷ���
sbit MOTOR = P1^0;     // ֱ�������������
sbit KEY1 = P3^1;      // ��������K1

// �����λѡ����
sbit DIG_WEI1 = P2^4;  // ����������λѡ
sbit DIG_WEI2 = P2^3;
sbit DIG_WEI3 = P2^2;

// ����ܶ�ѡ���壨��������ܣ�
#define DIG_SEG P0

// ��λ����
#define GEAR_0 0
#define GEAR_1 1
#define GEAR_2 2
#define GEAR_3 3
#define GEAR_4 4
#define GEAR_5 5

// ��������
void System_Init(void);
void Timer0_Init(void);
void Delay_ms(unsigned int ms);
void Display_Gear(unsigned char gear);
unsigned char Key_Scan(void);
void Motor_Control(unsigned char gear);

// ȫ�ֱ�������
extern unsigned char current_gear;
extern unsigned int timer0_count;

#endif