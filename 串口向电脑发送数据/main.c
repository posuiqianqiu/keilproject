#include "UART.h"

void main(void)
{
    UART_Init();
    Delayms(100);
    
    // �����ַ���"66"
    UART_SendString("66");
    
    while(1)
    {
        Delayms(1000);
    }
}