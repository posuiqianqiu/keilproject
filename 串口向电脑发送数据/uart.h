#ifndef __UART_H__
#define __UART_H__

#include <reg52.h>

// º¯ÊıÉùÃ÷
void UART_Init(void);
void UART_SendByte(unsigned char dat);
void UART_SendString(unsigned char *str);
void Delayms(unsigned int ms);

#endif