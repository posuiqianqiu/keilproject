#include "UART.h"

/**
 * @brief ���ڳ�ʼ��������������4800
 */
void UART_Init(void)
{
    SCON = 0x50;        // ���ڹ�����ʽ1��8λ���ݣ��ɱ䲨���ʣ��������
    TMOD &= 0x0F;       // �����ʱ��1ģʽλ
    TMOD |= 0x20;       // ��ʱ��1��������ʽ2��8λ�Զ���װ
    TH1 = 0xFA;         // ������4800�Ķ�ʱ����װֵ(11.0592MHz����)
    TL1 = 0xFA;         // ������4800�Ķ�ʱ����װֵ
    PCON &= 0x7F;       // SMOD=0�������ʲ��ӱ�
    TR1 = 1;            // ������ʱ��1
    ES = 0;             // �رմ����жϣ���ѯ��ʽ��
}

/**
 * @brief ���ڷ���һ���ֽ�
 * @param dat Ҫ���͵�����
 */
void UART_SendByte(unsigned char dat)
{
    SBUF = dat;         // �������뷢�ͻ�����
    while(!TI);         // �ȴ��������
    TI = 0;             // ��������жϱ�־
}

/**
 * @brief ���ڷ����ַ���
 * @param str Ҫ���͵��ַ���ָ��
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
 * @brief ���뼶��ʱ����
 * @param ms ��ʱ�ĺ�����
 */
void Delayms(unsigned int ms)
{
    unsigned int i, j;
    for(i = ms; i > 0; i--)
        for(j = 114; j > 0; j--);
}