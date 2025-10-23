#include "timer.h"
#include "lcd1602.h"

// 时间变量
unsigned char hour = 0;
unsigned char minute = 0;
unsigned char second = 0;

// 时间更新标志
bit time_update = 0;

// 定时器计数变量
unsigned int timer_count = 0;

// 定时器0初始化
void Timer0_Init(void)
{
    TMOD &= 0xF0;    // 清除T0的设置
    TMOD |= 0x01;    // 设置T0为模式1（16位定时器）
    
    // 计算12MHz下50ms的初值
    // 定时器计数 = 65536 - (50000) = 15536 = 0x3CB0
    TH0 = (65536 - 50000) / 256;
    TL0 = (65536 - 50000) % 256;
    
    ET0 = 1;    // 允许T0中断
    EA = 1;     // 开启总中断
}

// 启动定时器0
void Timer0_Start(void)
{
    TR0 = 1;
}

// 停止定时器0
void Timer0_Stop(void)
{
    TR0 = 0;
}

// 定时器0中断服务函数
void Timer0_ISR(void) interrupt 1
{
    // 重新装载初值
    TH0 = (65536 - 50000) / 256;
    TL0 = (65536 - 50000) % 256;
    
    timer_count++;
    
    // 50ms * 20 = 1000ms = 1s
    if(timer_count >= 20)
    {
        timer_count = 0;
        second++;
        
        if(second >= 60)
        {
            second = 0;
            minute++;
            
            if(minute >= 60)
            {
                minute = 0;
                hour++;
                
                if(hour >= 24)
                {
                    hour = 0;
                }
            }
        }
        
        time_update = 1;    // 设置时间更新标志
    }
}