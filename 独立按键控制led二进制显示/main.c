#include <reg52.h>  // 包含STC89C52RC头文件

sbit KEY = P3^1;    // 独立按键K1连接到P3.2

unsigned char count = 0;  // 计数变量，范围0-255

// 延时函数
void delay_ms(unsigned int ms)
{
    unsigned int i, j;
    for(i = 0; i < ms; i++)
        for(j = 0; j < 123; j++);
}

void main()
{
    P2 = 0xFF;      // 初始化P2口，所有LED熄灭
    KEY = 1;        // 设置按键引脚为输入模式
    
    while(1)
    {
        if(KEY == 0)        // 检测到按键按下
        {
            delay_ms(10);   // 延时消抖
            if(KEY == 0)    // 确认按键确实按下
            {
                count++;    // 计数器加1
                P2 = ~count; // 将计数值取反后输出到P2口
                // 注意：如果LED是低电平点亮，需要取反
                // 如果LED是高电平点亮，则直接使用 P2 = count;
                
                // 等待按键释放
                while(KEY == 0);
                delay_ms(10);   // 释放消抖
            }
        }
        
        // 这里可以添加其他需要持续执行的任务
    }
}