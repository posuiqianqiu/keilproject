#include <reg52.h>
#include "key.h"
#include "timer.h"

// LED引脚定义（假设LED连接在P2口）
#define LED_PORT P2

// 全局变量
unsigned int timer_count = 0;    // 定时计数器
bit flow_direction = 0;          // 流水方向：0-正向 1-反向
unsigned char led_pattern = 0xFE; // LED初始模式：1111 1110（D1亮）

/**
 * @brief 定时器中断服务函数
 * @param 无
 * @return 无
 */
void Timer0_UserISR(void)
{
    timer_count++;  // 定时计数器递增
    
    // 每1000ms（1秒）执行一次LED流水
    if(timer_count >= 1000)
    {
        timer_count = 0;  // 重置计数器
        
        // 根据方向更新LED模式
        if(flow_direction == 0)
        {
            // 正向流水：D1→D8
            led_pattern = (led_pattern << 1) | 0x01;
            if(led_pattern == 0xFF) 
                led_pattern = 0xFE;  // 回到起始模式
        }
        else
        {
            // 反向流水：D8→D1
            led_pattern = (led_pattern >> 1) | 0x80;
            if(led_pattern == 0x7F) 
                led_pattern = 0x7F;  // 调整边界
            if(led_pattern == 0xFF)
                led_pattern = 0x7F;  // 回到起始模式
        }
        
        // 更新LED显示
        LED_PORT = led_pattern;
    }
}

/**
 * @brief 系统初始化
 * @param 无
 * @return 无
 */
void System_Init(void)
{
    // 外设初始化
    Key_Init();
    Timer0_Init();
    Timer0_SetInterrupt(Timer0_UserISR);
    
    // 全局中断使能
    EA = 1;
    
    // 初始LED显示
    LED_PORT = led_pattern;
}

/**
 * @brief 主函数
 */
void main(void)
{
    // 系统初始化
    System_Init();
    
    while(1)
    {
        // 检测独立按键K1
        if(Key_Scan())
        {
            // 切换流水方向
            flow_direction = ~flow_direction;
            
            // 根据新方向重置LED模式
            if(flow_direction == 0)
            {
                led_pattern = 0xFE;  // 正向起始：D1亮
            }
            else
            {
                led_pattern = 0x7F;  // 反向起始：D8亮
            }
            
            LED_PORT = led_pattern;
        }
        
        // 主循环可以添加其他任务
        // 这里保持空循环，所有工作由中断处理
    }
}