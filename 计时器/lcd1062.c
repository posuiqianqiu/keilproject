#include "lcd1602.h"

// ��ʱ����
void LCD_Delay(unsigned int t)
{
    unsigned int i, j;
    for(i = 0; i < t; i++)
        for(j = 0; j < 120; j++);
}

// ���LCDæ״̬
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

// д���LCD
void LCD_WriteCmd(unsigned char cmd)
{
    LCD_CheckBusy();
    LCD_RS = 0;
    LCD_RW = 0;
    LCD_DATA = cmd;
    LCD_EN = 1;
    LCD_EN = 0;
}

// д���ݵ�LCD
void LCD_WriteData(unsigned char dat)
{
    LCD_CheckBusy();
    LCD_RS = 1;
    LCD_RW = 0;
    LCD_DATA = dat;
    LCD_EN = 1;
    LCD_EN = 0;
}

// LCD��ʼ��
void LCD_Init(void)
{
    LCD_WriteCmd(0x38);    // ����16x2��ʾ��5x7����8λ���ݽӿ�
    LCD_WriteCmd(0x0C);    // ��ʾ���������رգ������˸�ر�
    LCD_WriteCmd(0x06);    // ���ֲ���������Զ�����
    LCD_WriteCmd(0x01);    // ����
    LCD_Delay(2);
}

// ���ù��λ��
void LCD_SetCursor(unsigned char x, unsigned char y)
{
    unsigned char addr;
    if(y == 0)
        addr = 0x00 + x;
    else
        addr = 0x40 + x;
    LCD_WriteCmd(addr | 0x80);
}

// ��ʾ�ַ���
void LCD_ShowStr(unsigned char x, unsigned char y, unsigned char *str)
{
    LCD_SetCursor(x, y);
    while(*str != '\0')
    {
        LCD_WriteData(*str);
        str++;
    }
}

// ��ʾ���֣�00-59��ʽ��
void LCD_ShowNum(unsigned char x, unsigned char y, unsigned char num)
{
    unsigned char str[3];
    str[0] = num / 10 + '0';
    str[1] = num % 10 + '0';
    str[2] = '\0';
    LCD_ShowStr(x, y, str);
}