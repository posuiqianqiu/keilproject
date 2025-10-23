#include <reg52.h>
#include "lcd1602.h"
#include "timer.h"

// 显示时间函数
void Display_Time(void)
{
    LCD_ShowNum(0, 0, hour);      // 显示小时
    LCD_WriteData(':');           // 显示冒号
    LCD_ShowNum(3, 0, minute);    // 显示分钟
    LCD_WriteData(':');           // 显示冒号
    LCD_ShowNum(6, 0, second);    // 显示秒
}

// 主函数
void main(void)
{
    // LCD初始化
    LCD_Init();
    
    // 显示初始标题
    LCD_ShowStr(0, 1, "Digital Counter");
    
    // 显示初始时间 00:00:00
    Display_Time();
    
    // 定时器初始化
    Timer0_Init();
    
    // 启动定时器
    Timer0_Start();
    
    // 主循环
    while(1)
    {
        // 检查时间是否需要更新
        if(time_update)
        {
            time_update = 0;        // 清除更新标志
            Display_Time();         // 更新显示
        }
    }
}