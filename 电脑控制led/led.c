#include "LED.h"

/**
 * @brief LED��ʼ������
 */
void LED_Init(void)
{
    LED_PORT = 0xFF;    // ��ʼ״̬�ر�����LED
}

/**
 * @brief ����������ʾ����
 * @param num Ҫ��ʾ������(0-255)
 * 
 * ˵����LED1~LED8������P2.0~P2.7���͵�ƽ����
 * P2.0��ӦLED1�����������λ��
 * P2.7��ӦLED8�����������λ��
 * ���磺num=5 (������00000101) �����LED1��LED3
 */
void LED_DisplayBinary(unsigned char num)
{
    LED_PORT = ~num;    // ȡ������ΪLED�ǵ͵�ƽ����
}