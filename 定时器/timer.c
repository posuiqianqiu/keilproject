#include "timer.h"

// 定时器中断函数指针
void (*Timer0_ISR)(void) = 0;

/**
 * @brief 定时器0初始化
 * @param 无
 * @return 无
 * @说明 使用定时器0，模式1，16位定时，用于精确计时
 */
void Timer0_Init(void)
{
    TMOD &= 0xF0;   // 清除T0控制位
    TMOD |= 0x01;   // 设置T0为模式1（16位定时器）
    
    TH0 = T1MS >> 8;  // 设置定时初值高8位
    TL0 = T1MS & 0xFF; // 设置定时初值低8位
    
    ET0 = 1;        // 允许T0中断
    TR0 = 1;        // 启动T0
}

/**
 * @brief 设置定时器0中断服务函数
 * @param func 中断服务函数指针
 * @return 无
 */
void Timer0_SetInterrupt(void (*func)(void))
{
    Timer0_ISR = func;
}

/**
 * @brief 定时器0中断服务函数
 * @说明 中断号1，使用寄存器组1
 */
void Timer0_ISR_Handler(void) interrupt 1 using 1
{
    // 重装定时初值
    TH0 = T1MS >> 8;
    TL0 = T1MS & 0xFF;
    
    // 调用用户定义的中断服务函数
    if(Timer0_ISR != 0)
    {
        Timer0_ISR();
    }
}