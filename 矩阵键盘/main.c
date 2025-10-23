#include <reg52.h>
#include "matrixkey.h"
#include "lcd1602.h"

void main()
{
    unsigned char display_value;
    
    // 初始化
    MatrixKey_Init();
    LCD_Init();
    
    LCD_ShowString(0, 0, "Key Test:");
    LCD_ShowString(1, 0, "Key:   ");
    
    while(1)
    {
        // 处理按键（按下无现象，松手检测）
        MatrixKey_ProcessKey();
        
        // 获取需要显示的按键值
        display_value = MatrixKey_GetDisplayValue();
        
        if(display_value != 0)
        {
            // 在LCD上显示按键编号（两位数格式）
            if(display_value < 10)
            {
                LCD_ShowChar(1, 5, '0'); // 显示前导0
                LCD_ShowNum(1, 6, display_value, 1);
            }
            else
            {
                LCD_ShowNum(1, 5, display_value, 2);
            }
        }
        
    }
}