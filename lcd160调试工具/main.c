#include <reg52.h>
#include "lcd1602.h"

int Result = 1234 + (-567);

void main()
{
    LCD_Init();  // LCD初始化
    while(1)
    {
        Result++;
        LCD_Delay(1000000);
				LCD_ShowNum(0, 0, Result, 3);
    }
}