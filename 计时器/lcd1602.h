#ifndef _LCD1602_H_
#define _LCD1602_H_

#include <reg52.h>

// LCD数据端口定义
#define LCD_DATA P0

// LCD控制引脚定义（根据您的连接修改）
sbit LCD_RS = P2^6;
sbit LCD_RW = P2^5;
sbit LCD_EN = P2^7;

// 函数声明
void LCD_Init(void);
void LCD_WriteCmd(unsigned char cmd);
void LCD_WriteData(unsigned char dat);
void LCD_SetCursor(unsigned char x, unsigned char y);
void LCD_ShowStr(unsigned char x, unsigned char y, unsigned char *str);
void LCD_ShowNum(unsigned char x, unsigned char y, unsigned char num);

#endif