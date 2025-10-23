#include <reg52.h>

typedef unsigned int u16;
typedef unsigned char u8;

sbit LED1 = P2^0; // 将P2.0定义为LED1

void Delay(u16 time) // 延时函数
{
    u16 i, j;
    for(i = time; i > 0; i--)
        for(j = 110; j > 0; j--);
}

void main()
{
    while(1) // 主循环
    {
        LED1 = 0; // 点亮LED1
        Delay(500); // 延时500ms
        LED1 = 1; // 熄灭LED1
        Delay(500); // 延时500ms
    }
}