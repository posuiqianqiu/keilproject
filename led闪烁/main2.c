#include <reg51.h>  // 包含51单片机头文件

sbit LED = P2^0;    // 定义LED引脚为P2.0

// 延时函数
void delay_ms(unsigned int ms)
{
    unsigned int i, j;
    for(i = 0; i < ms; i++)
        for(j = 0; j < 123; j++);  // 12MHz晶振下的近似延时
}

void main()
{
    while(1)  // 主循环
    {
        LED = 0;        // P2.0输出低电平，LED亮
        delay_ms(500);  // 延时500ms
        
        LED = 1;        // P2.0输出高电平，LED灭
        delay_ms(500);  // 延时500ms
    }
}