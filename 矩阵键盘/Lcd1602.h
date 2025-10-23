#ifndef __LCD1602_H__
#define __LCD1602_H__

#include <reg52.h>

// 引脚定义
sbit LCD_RS = P2^6;  // 寄存器选择引脚 (0:命令寄存器, 1:数据寄存器)
sbit LCD_RW = P2^5;  // 读写控制引脚 (0:写, 1:读)
sbit LCD_EN = P2^7;  // 使能信号引脚
#define LCD_DATA P0   // 数据端口 (8位数据线)

// 函数声明

/**
 * @brief LCD1602延时函数
 * @param t 延时时间
 */
void LCD_Delay(unsigned int t);

/**
 * @brief 检查LCD忙状态
 * @return 0:不忙, 1:忙
 */
bit LCD_CheckBusy();

/**
 * @brief 向LCD写入命令
 * @param cmd 要写入的命令
 */
void LCD_WriteCmd(unsigned char cmd);

/**
 * @brief 向LCD写入数据
 * @param dat 要写入的数据
 */
void LCD_WriteData(unsigned char dat);

/**
 * @brief LCD1602初始化函数
 */
void LCD_Init();

/**
 * @brief 清屏函数
 */
void LCD_Clear();

/**
 * @brief 设置显示位置
 * @param x 行位置 (0-1)
 * @param y 列位置 (0-15)
 */
void LCD_SetCursor(unsigned char x, unsigned char y);

/**
 * @brief 显示字符串
 * @param x 行位置 (0-1)
 * @param y 列位置 (0-15)
 * @param str 要显示的字符串
 */
void LCD_ShowString(unsigned char x, unsigned char y, unsigned char *str);

/**
 * @brief 显示数字
 * @param x 行位置 (0-1)
 * @param y 列位置 (0-15)
 * @param num 要显示的数字 (0-65535)
 * @param len 显示的数字长度
 */
void LCD_ShowNum(unsigned char x, unsigned char y, unsigned int num, unsigned char len);

/**
 * @brief 显示有符号数字
 * @param x 行位置 (0-1)
 * @param y 列位置 (0-15)
 * @param num 要显示的数字 (-32768~32767)
 * @param len 显示的数字长度
 */
void LCD_ShowSignedNum(unsigned char x, unsigned char y, int num, unsigned char len);

/**
 * @brief 显示十六进制数字
 * @param x 行位置 (0-1)
 * @param y 列位置 (0-15)
 * @param num 要显示的数字 (0-0xFFFF)
 * @param len 显示的数字长度
 */
void LCD_ShowHexNum(unsigned char x, unsigned char y, unsigned int num, unsigned char len);

/**
 * @brief 显示字符
 * @param x 行位置 (0-1)
 * @param y 列位置 (0-15)
 * @param chr 要显示的字符
 */
void LCD_ShowChar(unsigned char x, unsigned char y, unsigned char chr);

/**
 * @brief 显示浮点数
 * @param x 行位置 (0-1)
 * @param y 列位置 (0-15)
 * @param fnum 要显示的浮点数
 * @param int_len 整数部分长度
 * @param dec_len 小数部分长度
 */
void LCD_ShowFloat(unsigned char x, unsigned char y, float fnum, unsigned char int_len, unsigned char dec_len);

#endif