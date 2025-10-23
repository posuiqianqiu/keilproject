#include "motor.h"

// ȫ�ֱ�������
unsigned char current_gear = 0;    // ��ǰ��λ
unsigned int timer0_count = 0;     // ��ʱ������
unsigned char pwm_duty = 0;        // PWMռ�ձ�

// ��������ܶ���� (0-5)
unsigned char code seg_table[] = {
    0x3F,  // 0
    0x06,  // 1  
    0x5B,  // 2
    0x4F,  // 3
    0x66,  // 4
    0x6D   // 5
};

/**
 * @brief ϵͳ��ʼ������
 */
void System_Init(void)
{
    MOTOR = 0;          // �����ʼ�ر�
    DIG_SEG = 0x00;     // ����ܳ�ʼ����ʾ
    
    // �����λѡ��ʼ�� - ֻ��������������
    DIG_WEI1 = 1;
    DIG_WEI2 = 1;
    DIG_WEI3 = 1;
    
    Timer0_Init();      // ��ʱ��0��ʼ��
}

/**
 * @brief ��ʱ��0��ʼ������
 * ����ģʽ1��16λ��ʱ��������PWM����
 */
void Timer0_Init(void)
{
    TMOD &= 0xF0;       // ���ö�ʱ��0ģʽ
    TMOD |= 0x01;       // ��ʱ��0����ģʽ1
    
    TH0 = 0xFC;         // ��ʱ1ms�ĳ�ֵ (11.0592MHz)
    TL0 = 0x66;
    
    ET0 = 1;            // ������ʱ��0�ж�
    EA = 1;             // �������ж�
    TR0 = 1;            // ������ʱ��0
}

/**
 * @brief ��ʱ����
 * @param ms ��ʱ������
 */
void Delay_ms(unsigned int ms)
{
    unsigned int i, j;
    for(i = 0; i < ms; i++)
        for(j = 0; j < 114; j++);
}

/**
 * @brief �������ʾ����
 * @param gear Ҫ��ʾ�ĵ�λ(0-5)
 */
void Display_Gear(unsigned char gear)
{
    if(gear <= 5) {
        DIG_SEG = seg_table[gear];  // ��ʾ��Ӧ����
    }
}

/**
 * @brief ����ɨ�躯��
 * @return ����״̬��1-�������£�0-�ް���
 */
unsigned char Key_Scan(void)
{
    static unsigned char key_flag = 0;  // ������־λ
    
    if(KEY1 == 0) {         // ��ⰴ���Ƿ���
        Delay_ms(10);       // ��ʱ����
        if(KEY1 == 0) {     // �ٴ�ȷ�ϰ�������
            if(key_flag == 0) {
                key_flag = 1;
                return 1;   // ���ذ��������ź�
            }
        }
    } else {
        key_flag = 0;       // �����ͷţ���־λ����
    }
    return 0;               // �ް�������
}

/**
 * @brief ������ƺ���
 * @param gear ��λ(0-5)
 */
void Motor_Control(unsigned char gear)
{
    switch(gear) {
        case GEAR_0:    // 0�� - ֹͣ
            pwm_duty = 0;
            break;
        case GEAR_1:    // 1�� - 20%ռ�ձ�
            pwm_duty = 20;
            break;
        case GEAR_2:    // 2�� - 40%ռ�ձ�
            pwm_duty = 40;
            break;
        case GEAR_3:    // 3�� - 60%ռ�ձ�
            pwm_duty = 60;
            break;
        case GEAR_4:    // 4�� - 80%ռ�ձ�
            pwm_duty = 80;
            break;
        case GEAR_5:    // 5�� - 100%ռ�ձ�
            pwm_duty = 100;
            break;
        default:        // Ĭ��ֹͣ
            pwm_duty = 0;
            break;
    }
}

/**
 * @brief ��ʱ��0�жϷ�����
 * ��������PWM�źſ��Ƶ��ת��
 */
void Timer0_ISR(void) interrupt 1
{
    static unsigned int pwm_count = 0;
    
    // ��װ��ʱ����ֵ
    TH0 = 0xFC;
    TL0 = 0x66;
    
    timer0_count++;
    pwm_count++;
    
    if(pwm_count >= 100) {  // PWM����Ϊ100����ʱ������
        pwm_count = 0;
    }
    
    // PWM�������
    if(pwm_count < pwm_duty) {
        MOTOR = 1;          // �ߵ�ƽ�����ת��
    } else {
        MOTOR = 0;          // �͵�ƽ�����ֹͣ
    }
}

/**
 * @brief ������
 */
void main(void)
{
    System_Init();          // ϵͳ��ʼ��
    Display_Gear(0);        // ��ʼ��ʾ0��
    
    while(1) {
        // �������
        if(Key_Scan()) {
            current_gear++;             // ��λ����
            if(current_gear > 5) {      // ��λѭ��
                current_gear = 0;
            }
            
            Motor_Control(current_gear);    // ���Ƶ��
            Display_Gear(current_gear);     // ������ʾ
            Delay_ms(300);                  // ������Ӧ��ʱ
        }
    }
}