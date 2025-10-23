#include <REG52.H>
#include "Delay.h"

sbit Buzzer = P2^5;  // ����������

// ����Ƶ�ʶ�Ӧ�Ķ�ʱ������ֵ��11.0592MHz����
#define L1 63777     // ����1 - ����ֵ
#define L2 63834     // ����2 - ����ֵ
#define L3 63927     // ����3 - ����ֵ
#define L4 63999     // ����4 - ����ֵ
#define L5 64080     // ����5 - ����ֵ
#define L6 64162     // ����6 - ����ֵ
#define L7 64224     // ����7 - ����ֵ

#define M1 64580     // ����1 - ����ֵ
#define M2 64684     // ����2 - ����ֵ
#define M3 64777     // ����3 - ����ֵ
#define M4 64820     // ����4 - ����ֵ
#define M5 64898     // ����5 - ����ֵ
#define M6 64968     // ����6 - ����ֵ
#define M7 65030     // ����7 - ����ֵ

#define H1 65058     // ����1 - ����ֵ
#define H2 65110     // ����2 - ����ֵ
#define H3 65157     // ����3 - ����ֵ
#define H4 65178     // ����4 - ����ֵ
#define H5 65217     // ����5 - ����ֵ
#define H6 65252     // ����6 - ����ֵ
#define H7 65283     // ����7 - ����ֵ

#define END 0        // ������־

// ����˼������ݣ�{����, ����}
unsigned int code Song[] = {
    M5,2, M3,2, M2,2, M3,2, M5,2, M3,2, M2,4,     // ��һ·������ͣͣ
    M5,2, M3,2, M2,2, M3,2, M1,4, M1,2,           // ˳������Ư���ĺۼ�
    M2,2, M3,2, M5,2, M6,2, M5,2, M3,2, M2,4,     // ������վ��ǰһ��
    M2,2, M3,2, M5,2, M3,2, M2,2, M1,2, L6,4,     // ����Щ��ԥ
    M1,2, M2,2, M3,2, M2,2, M1,2, L6,2, L5,4,     // ����Ц���������
    M1,2, M2,2, M3,2, M5,2, M2,4, M1,2,           // ���޿ɱ���
    M1,2, M2,2, M3,2, M5,2, M6,2, M5,2, M3,2,     // ����Ұ����
    M2,2, M3,2, M5,2, M3,2, M2,2, M1,2, L6,4,     // ������ôů
    
    // ��Ӹ������...
    M5,2, M6,2, H1,2, M6,2, M5,4, M3,2,           // �������ഺ��ӿ����
    M2,2, M3,2, M5,2, M3,2, M2,4, M1,2,           // Ҳ��ָ�ⵯ��ʢ��
    M1,2, M2,2, M3,2, M5,2, M6,2, H1,2, M6,2,     // ��֮���� �Ҿ���Եȥ��
    M5,4, M3,2, M2,4, M1,4,                       // ���Ź����� �η紵���
    
    END, END                                       // ����
};

unsigned int ToneDelay;    // ��ǰ�����Ķ�ʱ��ֵ
unsigned int ToneTime;     // ��ǰ�����ĳ���ʱ��

/**
  * @brief  ��ʱ��0��ʼ��
  * @param  ��
  * @retval ��
  */
void Timer0_Init()
{
    TMOD &= 0xF0;      // ���ö�ʱ��0Ϊģʽ1
    TMOD |= 0x01;
    TH0 = 0xFF;
    TL0 = 0xFF;
    ET0 = 1;           // ʹ�ܶ�ʱ��0�ж�
    EA = 1;            // �������ж�
}

/**
  * @brief  ����һ������
  * @param  freq ����Ƶ�ʶ�Ӧ�Ķ�ʱ��ֵ
  * @param  time ��������ʱ��
  * @retval ��
  */
void PlayNote(unsigned int freq, unsigned int time)
{
    if(freq == END) 
    {
        TR0 = 0;      // ֹͣ��ʱ��
        Buzzer = 1;   // �رշ�����
        return;
    }
    
    TR0 = 0;           // ֹͣ��ʱ��
    ToneDelay = freq;  // ���浱ǰ�����Ķ�ʱ��ֵ
    ToneTime = time;   // ���浱ǰ�����ĳ���ʱ��
    
    // ���ö�ʱ����ʼֵ
    TH0 = ToneDelay >> 8;
    TL0 = ToneDelay & 0xFF;
    
    TR0 = 1;           // ������ʱ��
}

/**
  * @brief  ������
  * @param  ��
  * @retval ��
  */
void main()
{
    unsigned int i = 0;
    Timer0_Init();     // ��ʼ����ʱ��0
    
    while(1)
    {
        // ���Ÿ���
        if(Song[i] != END)
        {
            PlayNote(Song[i], Song[i+1]);
            Delay(Song[i+1] * 120);  // �ȴ�����������ɣ�������ʱ����Ӧ11.0592MHz
            i += 2;
        }
        else
        {
            TR0 = 0;  // ֹͣ����
            Buzzer = 1; // �رշ�����
            i = 0;    // �ص���ͷ��׼�����²���
            Delay(2000); // ͣ��2������¿�ʼ
        }
    }
}

/**
  * @brief  ��ʱ��0�жϷ�����
  * @param  ��
  * @retval ��
  */
void Timer0_ISR() interrupt 1
{
    static unsigned int count = 0;
    
    // ���¼��ض�ʱ��ֵ
    TH0 = ToneDelay >> 8;
    TL0 = ToneDelay & 0xFF;
    
    // ��ת���������Ų�������
    Buzzer = !Buzzer;
    
    // ����ʱ�����
    count++;
    if(count >= ToneTime)
    {
        count = 0;
        TR0 = 0;      // ֹͣ��ʱ��
        Buzzer = 1;   // �رշ�����
    }
}