#include <reg52.h>

typedef unsigned int u16;
typedef unsigned char u8;

sbit LED1 = P2^0; // ��P2.0����ΪLED1

void Delay(u16 time) // ��ʱ����
{
    u16 i, j;
    for(i = time; i > 0; i--)
        for(j = 110; j > 0; j--);
}

void main()
{
    while(1) // ��ѭ��
    {
        LED1 = 0; // ����LED1
        Delay(500); // ��ʱ500ms
        LED1 = 1; // Ϩ��LED1
        Delay(500); // ��ʱ500ms
    }
}