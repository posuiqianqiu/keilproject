#ifndef __LED_H__
#define __LED_H__

#include <reg52.h>

// LED���Ŷ��� - LED������P2��
#define LED_PORT P2

// ��������
void LED_Init(void);
void LED_DisplayBinary(unsigned char num);

#endif