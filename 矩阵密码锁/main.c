#include <reg52.h>
#include "lcd1602.h"
#include "matrixkey.h"

// 密码相关定义
#define PASSWORD_LENGTH 4
#define MAX_ATTEMPTS 3

// 密码状态
unsigned char g_password[PASSWORD_LENGTH] = {0, 0, 0, 0};  // 当前输入密码
unsigned char g_input_count = 0;                           // 输入位数计数
unsigned char g_attempts = 0;                              // 尝试次数

// 正确密码 (4672)
const unsigned char g_correct_password[PASSWORD_LENGTH] = {4, 6, 7, 2};

// 函数声明
void System_Init(void);
void Display_Password(void);
void Process_Number_Key(unsigned char key_num);
void Process_Enter_Key(void);
void Process_Cancel_Key(void);
void Clear_Password(void);
void Show_Result(unsigned char is_correct);

/**
 * @brief 系统初始化
 */
void System_Init(void)
{
    // 外设初始化
    LCD_Init();
    MatrixKey_Init();
    
    // 状态初始化
    Clear_Password();
    g_attempts = 0;
    
    // 显示初始界面
    LCD_ShowString(0, 0, "Password Lock:");
    Display_Password();
}

/**
 * @brief 显示当前密码
 */
void Display_Password(void)
{
    LCD_ShowNum(1, 0, g_password[0], 1);
    LCD_ShowNum(1, 1, g_password[1], 1);
    LCD_ShowNum(1, 2, g_password[2], 1);
    LCD_ShowNum(1, 3, g_password[3], 1);
}

/**
 * @brief 处理数字按键
 * @param key_num 按键数值 (0-9)
 */
void Process_Number_Key(unsigned char key_num)
{
    // 如果已经输入了4位，先清除重新输入
    if(g_input_count >= PASSWORD_LENGTH)
    {
        Clear_Password();
    }
    
    // 存储数字并更新显示
    g_password[g_input_count] = key_num;
    g_input_count++;
    
    Display_Password();
}

/**
 * @brief 处理确认键
 */
void Process_Enter_Key(void)
{
    unsigned char i;
    unsigned char is_correct = 1;
    
    // 检查是否输入了完整的4位密码
    if(g_input_count != PASSWORD_LENGTH)
    {
        LCD_ShowString(1, 5, "Input 4 digits!");
        return;
    }
    
    // 比较密码
    for(i = 0; i < PASSWORD_LENGTH; i++)
    {
        if(g_password[i] != g_correct_password[i])
        {
            is_correct = 0;
            break;
        }
    }
    
    // 显示结果
    Show_Result(is_correct);
    
    // 更新尝试次数
    if(is_correct)
    {
        g_attempts = 0; // 成功时重置尝试次数
    }
    else
    {
        g_attempts++;
        if(g_attempts >= MAX_ATTEMPTS)
        {
            LCD_ShowString(0, 0, "Too many errors!");
            LCD_ShowString(1, 0, "System locked!  ");
            while(1); // 系统锁定
        }
    }
    
    // 清除密码准备下一次输入
    Clear_Password();
    Display_Password();
}

/**
 * @brief 处理取消键
 */
void Process_Cancel_Key(void)
{
    Clear_Password();
    Display_Password();
    LCD_ShowString(1, 5, "Canceled!     ");
}

/**
 * @brief 清除密码
 */
void Clear_Password(void)
{
    unsigned char i;
    for(i = 0; i < PASSWORD_LENGTH; i++)
    {
        g_password[i] = 0;
    }
    g_input_count = 0;
}

/**
 * @brief 显示验证结果
 * @param is_correct 密码是否正确
 */
void Show_Result(unsigned char is_correct)
{
    if(is_correct)
    {
        LCD_ShowString(1, 5, "OK   ");
        // 可以在这里添加开锁动作，比如控制继电器等
    }
    else
    {
        LCD_ShowString(1, 5, "ERR  ");
        LCD_ShowNum(1, 12, MAX_ATTEMPTS - g_attempts, 1);
        LCD_ShowChar(1, 13, ' ');
    }
}

/**
 * @brief 主函数
 */
void main()
{
    unsigned char key_value;
    
    // 系统初始化
    System_Init();
    
    while(1)
    {
        // 处理按键
        MatrixKey_ProcessKey();
        key_value = MatrixKey_GetDisplayValue();
        
        if(key_value != 0)
        {
            // 根据按键值进行处理
            if(key_value >= 1 && key_value <= 10)
            {
                // 数字键 S1-S10
                // S1=1, S2=2, ..., S9=9, S10=0
                unsigned char number;
                if(key_value == 10)
                    number = 0;  // S10 对应数字 0
                else
                    number = key_value;  // S1-S9 对应数字 1-9
                    
                Process_Number_Key(number);
            }
            else if(key_value == 11)
            {
                // 确认键 S11
                Process_Enter_Key();
            }
            else if(key_value == 12)
            {
                // 取消键 S12
                Process_Cancel_Key();
            }
            else
            {
                // S13-S16 未定义，可以忽略或添加其他功能
                LCD_ShowString(1, 5, "Invalid key! ");
            }
        }
       
    }
}