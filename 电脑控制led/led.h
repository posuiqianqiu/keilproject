#ifndef __LED_H__
#define __LED_H__

#include <reg52.h>

// LED引脚定义 - LED连接在P2口
#define LED_PORT P2

// 函数声明
void LED_Init(void);
void LED_DisplayBinary(unsigned char num);

#endif