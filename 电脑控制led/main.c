#include "UART.h"
#include "LED.h"
#include <string.h>
#include <stdlib.h>

// 全局变量
unsigned char received_data = 0;
unsigned char uart_buffer[10];
unsigned char buffer_index = 0;
bit number_received = 0;

/**
 * @brief 串口中断服务函数
 */
void UART_Interrupt(void) interrupt 4
{
    if(RI == 1)
    {
        RI = 0;
        unsigned char ch = SBUF;
        
        // 回显字符
        UART_SendByte(ch);
        
        // 如果是回车或换行，表示数字输入完成
        if(ch == '\r' || ch == '\n')
        {
            if(buffer_index > 0)
            {
                uart_buffer[buffer_index] = '\0'; // 字符串结束符
                number_received = 1; // 设置标志位
                buffer_index = 0;
            }
        }
        // 如果是数字，存入缓冲区
        else if(ch >= '0' && ch <= '9')
        {
            if(buffer_index < sizeof(uart_buffer)-1)
            {
                uart_buffer[buffer_index++] = ch;
            }
        }
        // 如果是退格键，删除最后一个字符
        else if(ch == 0x08 || ch == 0x7F) // 退格或删除键
        {
            if(buffer_index > 0)
            {
                buffer_index--;
                UART_SendByte(' ');
                UART_SendByte(0x08);
            }
        }
    }
    
    if(TI == 1)
    {
        TI = 0;
    }
}

/**
 * @brief 将字符串转换为数字
 * @param str 数字字符串
 * @return 转换后的数字
 */
unsigned char StringToNumber(unsigned char *str)
{
    unsigned char result = 0;
    while(*str >= '0' && *str <= '9')
    {
        result = result * 10 + (*str - '0');
        str++;
    }
    return result;
}

void main(void)
{
    UART_Init();
    LED_Init();
    
    UART_SendString("=== LED二进制显示器 ===\r\n");
    UART_SendString("请输入0-255之间的数字，按回车结束:\r\n");
    UART_SendString("例如: 输入255然后按回车\r\n");
    UART_SendString("=======================\r\n");
    
    while(1)
    {
        if(number_received)
        {
            number_received = 0;
            
            // 将字符串转换为数字
            received_data = StringToNumber(uart_buffer);
            
            UART_SendString("\r\n转换后的数字: ");
            UART_SendByte('0' + (received_data / 100));
            UART_SendByte('0' + ((received_data / 10) % 10));
            UART_SendByte('0' + (received_data % 10));
            UART_SendString(" (0x");
            
            // 发送十六进制表示
            unsigned char high = received_data / 16;
            unsigned char low = received_data % 16;
            UART_SendByte(high < 10 ? '0' + high : 'A' + high - 10);
            UART_SendByte(low < 10 ? '0' + low : 'A' + low - 10);
            UART_SendString(")\r\n");
            
            UART_SendString("二进制: ");
            // 发送二进制表示
            unsigned char i;
            for(i = 0; i < 8; i++)
            {
                if(received_data & (0x80 >> i))
                    UART_SendByte('1');
                else
                    UART_SendByte('0');
            }
            UART_SendString("\r\n");
            
            // 控制LED显示
            LED_DisplayBinary(received_data);
            
            UART_SendString("请继续输入数字: ");
        }
        
        Delayms(10);
    }
}