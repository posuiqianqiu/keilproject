#include "LED.h"

/**
 * @brief LED初始化函数
 */
void LED_Init(void)
{
    LED_PORT = 0xFF;    // 初始状态关闭所有LED
}

/**
 * @brief 按二进制显示数字
 * @param num 要显示的数字(0-255)
 * 
 * 说明：LED1~LED8连接在P2.0~P2.7，低电平点亮
 * P2.0对应LED1（二进制最低位）
 * P2.7对应LED8（二进制最高位）
 * 例如：num=5 (二进制00000101) 会点亮LED1和LED3
 */
void LED_DisplayBinary(unsigned char num)
{
    LED_PORT = ~num;    // 取反，因为LED是低电平点亮
}