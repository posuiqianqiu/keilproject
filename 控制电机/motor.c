#include "motor.h"

// 全局变量定义
unsigned char current_gear = 0;    // 当前档位
unsigned int timer0_count = 0;     // 定时器计数
unsigned char pwm_duty = 0;        // PWM占空比

// 共阴数码管段码表 (0-5)
unsigned char code seg_table[] = {
    0x3F,  // 0
    0x06,  // 1  
    0x5B,  // 2
    0x4F,  // 3
    0x66,  // 4
    0x6D   // 5
};

/**
 * @brief 系统初始化函数
 */
void System_Init(void)
{
    MOTOR = 0;          // 电机初始关闭
    DIG_SEG = 0x00;     // 数码管初始不显示
    
    // 数码管位选初始化 - 只开启最左侧数码管
    DIG_WEI1 = 1;
    DIG_WEI2 = 1;
    DIG_WEI3 = 1;
    
    Timer0_Init();      // 定时器0初始化
}

/**
 * @brief 定时器0初始化函数
 * 工作模式1，16位定时器，用于PWM生成
 */
void Timer0_Init(void)
{
    TMOD &= 0xF0;       // 设置定时器0模式
    TMOD |= 0x01;       // 定时器0工作模式1
    
    TH0 = 0xFC;         // 定时1ms的初值 (11.0592MHz)
    TL0 = 0x66;
    
    ET0 = 1;            // 开启定时器0中断
    EA = 1;             // 开启总中断
    TR0 = 1;            // 启动定时器0
}

/**
 * @brief 延时函数
 * @param ms 延时毫秒数
 */
void Delay_ms(unsigned int ms)
{
    unsigned int i, j;
    for(i = 0; i < ms; i++)
        for(j = 0; j < 114; j++);
}

/**
 * @brief 数码管显示函数
 * @param gear 要显示的档位(0-5)
 */
void Display_Gear(unsigned char gear)
{
    if(gear <= 5) {
        DIG_SEG = seg_table[gear];  // 显示对应数字
    }
}

/**
 * @brief 按键扫描函数
 * @return 按键状态：1-按键按下，0-无按键
 */
unsigned char Key_Scan(void)
{
    static unsigned char key_flag = 0;  // 按键标志位
    
    if(KEY1 == 0) {         // 检测按键是否按下
        Delay_ms(10);       // 延时消抖
        if(KEY1 == 0) {     // 再次确认按键按下
            if(key_flag == 0) {
                key_flag = 1;
                return 1;   // 返回按键按下信号
            }
        }
    } else {
        key_flag = 0;       // 按键释放，标志位清零
    }
    return 0;               // 无按键按下
}

/**
 * @brief 电机控制函数
 * @param gear 档位(0-5)
 */
void Motor_Control(unsigned char gear)
{
    switch(gear) {
        case GEAR_0:    // 0档 - 停止
            pwm_duty = 0;
            break;
        case GEAR_1:    // 1档 - 20%占空比
            pwm_duty = 20;
            break;
        case GEAR_2:    // 2档 - 40%占空比
            pwm_duty = 40;
            break;
        case GEAR_3:    // 3档 - 60%占空比
            pwm_duty = 60;
            break;
        case GEAR_4:    // 4档 - 80%占空比
            pwm_duty = 80;
            break;
        case GEAR_5:    // 5档 - 100%占空比
            pwm_duty = 100;
            break;
        default:        // 默认停止
            pwm_duty = 0;
            break;
    }
}

/**
 * @brief 定时器0中断服务函数
 * 用于生成PWM信号控制电机转速
 */
void Timer0_ISR(void) interrupt 1
{
    static unsigned int pwm_count = 0;
    
    // 重装定时器初值
    TH0 = 0xFC;
    TL0 = 0x66;
    
    timer0_count++;
    pwm_count++;
    
    if(pwm_count >= 100) {  // PWM周期为100个定时器周期
        pwm_count = 0;
    }
    
    // PWM输出控制
    if(pwm_count < pwm_duty) {
        MOTOR = 1;          // 高电平，电机转动
    } else {
        MOTOR = 0;          // 低电平，电机停止
    }
}

/**
 * @brief 主函数
 */
void main(void)
{
    System_Init();          // 系统初始化
    Display_Gear(0);        // 初始显示0档
    
    while(1) {
        // 按键检测
        if(Key_Scan()) {
            current_gear++;             // 档位增加
            if(current_gear > 5) {      // 档位循环
                current_gear = 0;
            }
            
            Motor_Control(current_gear);    // 控制电机
            Display_Gear(current_gear);     // 更新显示
            Delay_ms(300);                  // 按键响应延时
        }
    }
}