#include "UART.h"
#include "LED.h"
#include <string.h>
#include <stdlib.h>

// ȫ�ֱ���
unsigned char received_data = 0;
unsigned char uart_buffer[10];
unsigned char buffer_index = 0;
bit number_received = 0;

/**
 * @brief �����жϷ�����
 */
void UART_Interrupt(void) interrupt 4
{
    if(RI == 1)
    {
        RI = 0;
        unsigned char ch = SBUF;
        
        // �����ַ�
        UART_SendByte(ch);
        
        // ����ǻس����У���ʾ�����������
        if(ch == '\r' || ch == '\n')
        {
            if(buffer_index > 0)
            {
                uart_buffer[buffer_index] = '\0'; // �ַ���������
                number_received = 1; // ���ñ�־λ
                buffer_index = 0;
            }
        }
        // ��������֣����뻺����
        else if(ch >= '0' && ch <= '9')
        {
            if(buffer_index < sizeof(uart_buffer)-1)
            {
                uart_buffer[buffer_index++] = ch;
            }
        }
        // ������˸����ɾ�����һ���ַ�
        else if(ch == 0x08 || ch == 0x7F) // �˸��ɾ����
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
 * @brief ���ַ���ת��Ϊ����
 * @param str �����ַ���
 * @return ת���������
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
    
    UART_SendString("=== LED��������ʾ�� ===\r\n");
    UART_SendString("������0-255֮������֣����س�����:\r\n");
    UART_SendString("����: ����255Ȼ�󰴻س�\r\n");
    UART_SendString("=======================\r\n");
    
    while(1)
    {
        if(number_received)
        {
            number_received = 0;
            
            // ���ַ���ת��Ϊ����
            received_data = StringToNumber(uart_buffer);
            
            UART_SendString("\r\nת���������: ");
            UART_SendByte('0' + (received_data / 100));
            UART_SendByte('0' + ((received_data / 10) % 10));
            UART_SendByte('0' + (received_data % 10));
            UART_SendString(" (0x");
            
            // ����ʮ�����Ʊ�ʾ
            unsigned char high = received_data / 16;
            unsigned char low = received_data % 16;
            UART_SendByte(high < 10 ? '0' + high : 'A' + high - 10);
            UART_SendByte(low < 10 ? '0' + low : 'A' + low - 10);
            UART_SendString(")\r\n");
            
            UART_SendString("������: ");
            // ���Ͷ����Ʊ�ʾ
            unsigned char i;
            for(i = 0; i < 8; i++)
            {
                if(received_data & (0x80 >> i))
                    UART_SendByte('1');
                else
                    UART_SendByte('0');
            }
            UART_SendString("\r\n");
            
            // ����LED��ʾ
            LED_DisplayBinary(received_data);
            
            UART_SendString("�������������: ");
        }
        
        Delayms(10);
    }
}