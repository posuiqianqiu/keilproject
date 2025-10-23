#include "timer.h"
#include "lcd1602.h"

// ʱ�����
unsigned char hour = 0;
unsigned char minute = 0;
unsigned char second = 0;

// ʱ����±�־
bit time_update = 0;

// ��ʱ����������
unsigned int timer_count = 0;

// ��ʱ��0��ʼ��
void Timer0_Init(void)
{
    TMOD &= 0xF0;    // ���T0������
    TMOD |= 0x01;    // ����T0Ϊģʽ1��16λ��ʱ����
    
    // ����12MHz��50ms�ĳ�ֵ
    // ��ʱ������ = 65536 - (50000) = 15536 = 0x3CB0
    TH0 = (65536 - 50000) / 256;
    TL0 = (65536 - 50000) % 256;
    
    ET0 = 1;    // ����T0�ж�
    EA = 1;     // �������ж�
}

// ������ʱ��0
void Timer0_Start(void)
{
    TR0 = 1;
}

// ֹͣ��ʱ��0
void Timer0_Stop(void)
{
    TR0 = 0;
}

// ��ʱ��0�жϷ�����
void Timer0_ISR(void) interrupt 1
{
    // ����װ�س�ֵ
    TH0 = (65536 - 50000) / 256;
    TL0 = (65536 - 50000) % 256;
    
    timer_count++;
    
    // 50ms * 20 = 1000ms = 1s
    if(timer_count >= 20)
    {
        timer_count = 0;
        second++;
        
        if(second >= 60)
        {
            second = 0;
            minute++;
            
            if(minute >= 60)
            {
                minute = 0;
                hour++;
                
                if(hour >= 24)
                {
                    hour = 0;
                }
            }
        }
        
        time_update = 1;    // ����ʱ����±�־
    }
}