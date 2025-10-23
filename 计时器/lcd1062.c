#include "lcd1602.h"

// 延时函数
void LCD_Delay(unsigned int t)
{
    unsigned int i, j;
    for(i = 0; i < t; i++)
        for(j = 0; j < 120; j++);
}

// 检查LCD忙状态
void LCD_CheckBusy(void)
{
    unsigned char sta;
    LCD_DATA = 0xFF;
    LCD_RS = 0;
    LCD_RW = 1;
    do {
        LCD_EN = 1;
        sta = LCD_DATA;
        LCD_EN = 0;
    } while(sta & 0x80);
}

// 写命令到LCD
void LCD_WriteCmd(unsigned char cmd)
{
    LCD_CheckBusy();
    LCD_RS = 0;
    LCD_RW = 0;
    LCD_DATA = cmd;
    LCD_EN = 1;
    LCD_EN = 0;
}

// 写数据到LCD
void LCD_WriteData(unsigned char dat)
{
    LCD_CheckBusy();
    LCD_RS = 1;
    LCD_RW = 0;
    LCD_DATA = dat;
    LCD_EN = 1;
    LCD_EN = 0;
}

// LCD初始化
void LCD_Init(void)
{
    LCD_WriteCmd(0x38);    // 设置16x2显示，5x7点阵，8位数据接口
    LCD_WriteCmd(0x0C);    // 显示器开，光标关闭，光标闪烁关闭
    LCD_WriteCmd(0x06);    // 文字不动，光标自动右移
    LCD_WriteCmd(0x01);    // 清屏
    LCD_Delay(2);
}

// 设置光标位置
void LCD_SetCursor(unsigned char x, unsigned char y)
{
    unsigned char addr;
    if(y == 0)
        addr = 0x00 + x;
    else
        addr = 0x40 + x;
    LCD_WriteCmd(addr | 0x80);
}

// 显示字符串
void LCD_ShowStr(unsigned char x, unsigned char y, unsigned char *str)
{
    LCD_SetCursor(x, y);
    while(*str != '\0')
    {
        LCD_WriteData(*str);
        str++;
    }
}

// 显示数字（00-59格式）
void LCD_ShowNum(unsigned char x, unsigned char y, unsigned char num)
{
    unsigned char str[3];
    str[0] = num / 10 + '0';
    str[1] = num % 10 + '0';
    str[2] = '\0';
    LCD_ShowStr(x, y, str);
}