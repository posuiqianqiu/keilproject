#include <REG52.H>
#include "Delay.h"

sbit Buzzer = P2^5;  // 蜂鸣器引脚

// 音符频率对应的定时器重载值（11.0592MHz晶振）
#define L1 63777     // 低音1 - 修正值
#define L2 63834     // 低音2 - 修正值
#define L3 63927     // 低音3 - 修正值
#define L4 63999     // 低音4 - 修正值
#define L5 64080     // 低音5 - 修正值
#define L6 64162     // 低音6 - 修正值
#define L7 64224     // 低音7 - 修正值

#define M1 64580     // 中音1 - 修正值
#define M2 64684     // 中音2 - 修正值
#define M3 64777     // 中音3 - 修正值
#define M4 64820     // 中音4 - 修正值
#define M5 64898     // 中音5 - 修正值
#define M6 64968     // 中音6 - 修正值
#define M7 65030     // 中音7 - 修正值

#define H1 65058     // 高音1 - 修正值
#define H2 65110     // 高音2 - 修正值
#define H3 65157     // 高音3 - 修正值
#define H4 65178     // 高音4 - 修正值
#define H5 65217     // 高音5 - 修正值
#define H6 65252     // 高音6 - 修正值
#define H7 65283     // 高音7 - 修正值

#define END 0        // 结束标志

// 起风了简谱数据：{音符, 节拍}
unsigned int code Song[] = {
    M5,2, M3,2, M2,2, M3,2, M5,2, M3,2, M2,4,     // 这一路上走走停停
    M5,2, M3,2, M2,2, M3,2, M1,4, M1,2,           // 顺着少年漂流的痕迹
    M2,2, M3,2, M5,2, M6,2, M5,2, M3,2, M2,4,     // 迈出车站的前一刻
    M2,2, M3,2, M5,2, M3,2, M2,2, M1,2, L6,4,     // 竟有些犹豫
    M1,2, M2,2, M3,2, M2,2, M1,2, L6,2, L5,4,     // 不禁笑这近乡情怯
    M1,2, M2,2, M3,2, M5,2, M2,4, M1,2,           // 仍无可避免
    M1,2, M2,2, M3,2, M5,2, M6,2, M5,2, M3,2,     // 而长野的天
    M2,2, M3,2, M5,2, M3,2, M2,2, M1,2, L6,4,     // 依旧那么暖
    
    // 添加更多段落...
    M5,2, M6,2, H1,2, M6,2, M5,4, M3,2,           // 我曾将青春翻涌成她
    M2,2, M3,2, M5,2, M3,2, M2,4, M1,2,           // 也曾指尖弹出盛夏
    M1,2, M2,2, M3,2, M5,2, M6,2, H1,2, M6,2,     // 心之所动 且就随缘去吧
    M5,4, M3,2, M2,4, M1,4,                       // 逆着光行走 任风吹雨打
    
    END, END                                       // 结束
};

unsigned int ToneDelay;    // 当前音符的定时器值
unsigned int ToneTime;     // 当前音符的持续时间

/**
  * @brief  定时器0初始化
  * @param  无
  * @retval 无
  */
void Timer0_Init()
{
    TMOD &= 0xF0;      // 设置定时器0为模式1
    TMOD |= 0x01;
    TH0 = 0xFF;
    TL0 = 0xFF;
    ET0 = 1;           // 使能定时器0中断
    EA = 1;            // 开启总中断
}

/**
  * @brief  播放一个音符
  * @param  freq 音符频率对应的定时器值
  * @param  time 音符持续时间
  * @retval 无
  */
void PlayNote(unsigned int freq, unsigned int time)
{
    if(freq == END) 
    {
        TR0 = 0;      // 停止定时器
        Buzzer = 1;   // 关闭蜂鸣器
        return;
    }
    
    TR0 = 0;           // 停止定时器
    ToneDelay = freq;  // 保存当前音符的定时器值
    ToneTime = time;   // 保存当前音符的持续时间
    
    // 设置定时器初始值
    TH0 = ToneDelay >> 8;
    TL0 = ToneDelay & 0xFF;
    
    TR0 = 1;           // 启动定时器
}

/**
  * @brief  主函数
  * @param  无
  * @retval 无
  */
void main()
{
    unsigned int i = 0;
    Timer0_Init();     // 初始化定时器0
    
    while(1)
    {
        // 播放歌曲
        if(Song[i] != END)
        {
            PlayNote(Song[i], Song[i+1]);
            Delay(Song[i+1] * 120);  // 等待音符播放完成，调整延时以适应11.0592MHz
            i += 2;
        }
        else
        {
            TR0 = 0;  // 停止播放
            Buzzer = 1; // 关闭蜂鸣器
            i = 0;    // 回到开头，准备重新播放
            Delay(2000); // 停顿2秒后重新开始
        }
    }
}

/**
  * @brief  定时器0中断服务函数
  * @param  无
  * @retval 无
  */
void Timer0_ISR() interrupt 1
{
    static unsigned int count = 0;
    
    // 重新加载定时器值
    TH0 = ToneDelay >> 8;
    TL0 = ToneDelay & 0xFF;
    
    // 翻转蜂鸣器引脚产生方波
    Buzzer = !Buzzer;
    
    // 音符时间控制
    count++;
    if(count >= ToneTime)
    {
        count = 0;
        TR0 = 0;      // 停止定时器
        Buzzer = 1;   // 关闭蜂鸣器
    }
}