#include "key.h"

/**
 * @brief 独立按键初始化
 * @param 无
 * @return 无
 */
void Key_Init(void)
{
    // P3口默认有上拉电阻，无需额外初始化
}

/**
 * @brief 独立按键扫描函数（带消抖）
 * @param 无
 * @return 1:按键按下 0:无按键
 */
bit Key_Scan(void)
{
    static bit key_state = 0;  // 按键状态记录
    
    if(K1 == 0)  // 检测按键是否按下
    {
        // 延时消抖
        {
            unsigned int i;
            for(i = 0; i < 1000; i++);
        }
        
        if(K1 == 0)  // 再次确认按键按下
        {
            if(key_state == 0)  // 检测按键状态变化（下降沿）
            {
                key_state = 1;
                return 1;  // 返回按键按下信号
            }
        }
    }
    else
    {
        key_state = 0;  // 按键释放
    }
    
    return 0;  // 无按键按下
}