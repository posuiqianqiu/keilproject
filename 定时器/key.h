#ifndef KEY_H
#define KEY_H

#include <reg52.h>

// 独立按键引脚定义
sbit K1 = P3^1;

// 函数声明
void Key_Init(void);
bit Key_Scan(void);

#endif