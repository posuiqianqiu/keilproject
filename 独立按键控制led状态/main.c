#include <reg52.h>  // 包含STC89C52RC头文件

sbit LED = P2^0;    // LED连接到P2.0
sbit KEY = P3^1;    // 独立按键K1连接到P3.2

bit led_state = 1;   // LED状态标志，1表示熄灭，0表示亮起

// 延时函数
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
        if(KEY == 0)        // 检测到按键按下
        {
            delay_ms(10);   // 延时消抖
            if(KEY == 0)    // 确认按键确实按下
            {
                // 切换LED状态
                led_state =~ led_state;
                LED = led_state;
                
                // 等待按键释放
                while(KEY == 0)
                {
                    // 等待期间可以执行其他任务
                }
                delay_ms(10);   // 释放消抖
            }
        }
        
        // 这里可以添加其他需要持续执行的任务
    }
}