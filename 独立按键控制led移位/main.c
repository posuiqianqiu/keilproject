#include <reg52.h>
#include <intrins.h>  // 包含循环移位函数

sbit K1 = P3^1;     // 右移按键
sbit K2 = P3^0;     // 左移按键

unsigned char led_pattern = 0x01;  // LED模式

void delay_ms(unsigned int ms)
{
    unsigned int i, j;
    for(i = 0; i < ms; i++)
        for(j = 0; j < 123; j++);
}

// K1按键检测函数
bit key_scan_K1(void)
{
    if(K1 == 0)
    {
        delay_ms(10);
        if(K1 == 0)
        {
            while(!K1);    // 等待按键释放
            delay_ms(10);
            return 1;
        }
    }
    return 0;
}

// K2按键检测函数
bit key_scan_K2(void)
{
    if(K2 == 0)
    {
        delay_ms(10);
        if(K2 == 0)
        {
            while(!K2);    // 等待按键释放
            delay_ms(10);
            return 1;
        }
    }
    return 0;
}

void main()
{
    P2 = 0xFF;      // 初始所有LED熄灭
    K1 = 1;
    K2 = 1;
    
    P2 = ~led_pattern;  // 初始显示
    
    while(1)
    {
        if(key_scan_K1())
        {
            // 循环右移
            led_pattern = _cror_(led_pattern, 1);
            P2 = ~led_pattern;
        }
        
        if(key_scan_K2())
        {
            // 循环左移
            led_pattern = _crol_(led_pattern, 1);
            P2 = ~led_pattern;
        }
    }
}