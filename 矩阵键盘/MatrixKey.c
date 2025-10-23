#include "matrixkey.h"
#include <intrins.h>

// 按键显示缓冲区
unsigned char KeyDisplayValue = 0;

/**
 * @brief 简单延时函数
 * @param t 延时时间
 * @return 无
 */
void Key_Delay(unsigned int t)
{
    while(t--);
}

/**
 * @brief 矩阵键盘初始化
 * @param 无
 * @return 无
 */
void MatrixKey_Init(void)
{
    KEY_PORT = 0xFF;  // 初始化P1口为高电平
    KeyDisplayValue = 0; // 初始化显示值为0
}

/**
 * @brief 行扫描法检测按键
 * @param 无
 * @return 按键值 (1-16) 或 0(无按键)
 * @说明 修正行列映射，确保左上角S1对应01
 */
unsigned char MatrixKey_RowScan(void)
{
    unsigned char row, col;
    unsigned char key_value = 0;
    unsigned char temp;
    
    // 行扫描：逐行输出低电平
    for(row = 0; row < 4; row++)
    {
        // 当前行输出0，其他行输出1
        // 行线在P1.7-P1.4，所以左移(4+row)位
        KEY_PORT = (0xFF & ~(1 << (4 + row)));
        
        // 短暂延时，等待信号稳定
        _nop_();
        _nop_();
        
        // 读取列线状态（低4位 P1.3-P1.0）
        temp = KEY_PORT & 0x0F;
        
        // 检查哪一列为低电平
        if(temp != 0x0F)  // 有列线被拉低
        {
            // 确定具体的列
            for(col = 0; col < 4; col++)
            {
                if(!(temp & (1 << col)))
                {
                    key_value = (3 - row) * 4 + (3 - col) + 1;
                    
                    // 恢复端口状态
                    KEY_PORT = 0xFF;
                    return key_value;
                }
            }
        }
    }
    
    // 恢复端口状态
    KEY_PORT = 0xFF;
    return 0;  // 无按键按下
}

/**
 * @brief 等待按键释放
 * @param 无
 * @return 无
 */
void MatrixKey_WaitRelease(void)
{
    // 等待所有按键释放
    while(MatrixKey_RowScan() != 0)
    {
        Key_Delay(100);
    }
    Key_Delay(100); // 额外延时确保稳定
}

/**
 * @brief 获取按键并处理显示
 * @param 无
 * @return 无
 * @说明 主功能函数：按下无现象，松手显示对应编号
 */
void MatrixKey_ProcessKey(void)
{
    static unsigned char last_key = 0;
    unsigned char current_key;
    
    // 检测当前按键状态
    current_key = MatrixKey_RowScan();
    
    if(current_key != 0)
    {
        // 有按键按下，记录按键值但不显示
        last_key = current_key;
    }
    else if(last_key != 0)
    {
        // 按键从按下变为释放，显示对应编号
        KeyDisplayValue = last_key;
        
        // 重置last_key，避免重复触发
        last_key = 0;
        
        // 确保按键完全释放
        MatrixKey_WaitRelease();
    }
}

/**
 * @brief 获取要显示的按键值
 * @param 无
 * @return 要显示的按键编号 (0-16)
 * @说明 返回0表示无显示，1-16表示对应按键编号
 */
unsigned char MatrixKey_GetDisplayValue(void)
{
    unsigned char temp = KeyDisplayValue;
    KeyDisplayValue = 0; // 读取后清零，避免重复显示
    return temp;
}