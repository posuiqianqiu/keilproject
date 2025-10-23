#include <reg52.h>

sbit LED = P2^0;    // LED连接到P2.0
sbit KEY = P3^1;    // 独立按键K1连接到P3.2

// 简单延时函数
void delay_ms(unsigned int ms)
{
    unsigned int i, j;
    for(i = 0; i < ms; i++)
        for(j = 0; j < 123; j++);
}

void main()
{
    LED = 1;        // 初始化LED熄灭
    KEY = 1;        // 设置按键引脚为输入模式
    
    while(1)
    {
        if(KEY == 0)        // 第一次检测到按键按下
        {
            delay_ms(10);   // 延时10ms消抖
            if(KEY == 0)    // 再次确认按键确实按下
            {
                LED = 0;    // 按键按下，LED亮
                
                // 等待按键释放
                while(KEY == 0)
                {
                    // 可以在这里添加其他功能
                }
                delay_ms(10);   // 释放消抖
            }
        }
        else
        {
            LED = 1;    // 按键松开，LED灭
        }
    }
}