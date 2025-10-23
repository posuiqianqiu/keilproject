#include <reg52.h>
#include "lcd1602.h"
#include "timer.h"

// ��ʾʱ�亯��
void Display_Time(void)
{
    LCD_ShowNum(0, 0, hour);      // ��ʾСʱ
    LCD_WriteData(':');           // ��ʾð��
    LCD_ShowNum(3, 0, minute);    // ��ʾ����
    LCD_WriteData(':');           // ��ʾð��
    LCD_ShowNum(6, 0, second);    // ��ʾ��
}

// ������
void main(void)
{
    // LCD��ʼ��
    LCD_Init();
    
    // ��ʾ��ʼ����
    LCD_ShowStr(0, 1, "Digital Counter");
    
    // ��ʾ��ʼʱ�� 00:00:00
    Display_Time();
    
    // ��ʱ����ʼ��
    Timer0_Init();
    
    // ������ʱ��
    Timer0_Start();
    
    // ��ѭ��
    while(1)
    {
        // ���ʱ���Ƿ���Ҫ����
        if(time_update)
        {
            time_update = 0;        // ������±�־
            Display_Time();         // ������ʾ
        }
    }
}