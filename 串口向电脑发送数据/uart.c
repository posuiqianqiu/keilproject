#include "UART.h"

/**
 * @brief 串口初始化函数，波特率4800
 */
void UART_Init(void)
{
    SCON = 0x50;        // 串口工作方式1，8位数据，可变波特率，允许接收
    TMOD &= 0x0F;       // 清除定时器1模式位
    TMOD |= 0x20;       // 定时器1，工作方式2，8位自动重装
    TH1 = 0xFA;         // 波特率4800的定时器重装值(11.0592MHz晶振)
    TL1 = 0xFA;         // 波特率4800的定时器重装值
    PCON &= 0x7F;       // SMOD=0，波特率不加倍
    TR1 = 1;            // 启动定时器1
    ES = 0;             // 关闭串口中断（查询方式）
}

/**
 * @brief 串口发送一个字节
 * @param dat 要发送的数据
 */
void UART_SendByte(unsigned char dat)
{
    SBUF = dat;         // 数据送入发送缓冲器
    while(!TI);         // 等待发送完成
    TI = 0;             // 清除发送中断标志
}

/**
 * @brief 串口发送字符串
 * @param str 要发送的字符串指针
 */
void UART_SendString(unsigned char *str)
{
    while(*str != '\0')
    {
        UART_SendByte(*str);
        str++;
    }
}

/**
 * @brief 毫秒级延时函数
 * @param ms 延时的毫秒数
 */
void Delayms(unsigned int ms)
{
    unsigned int i, j;
    for(i = ms; i > 0; i--)
        for(j = 114; j > 0; j--);
}