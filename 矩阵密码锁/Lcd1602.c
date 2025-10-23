#include "lcd1602.h"
#include <intrins.h>

// 自定义延时函数，用于LCD时序控制
void LCD_Delay(unsigned int t)
{
    while(t--);
}

// 检查LCD忙状态
bit LCD_CheckBusy()
{
    bit result;
    LCD_DATA = 0xFF;    // 准备读取数据
    LCD_RS = 0;         // 选择命令寄存器
    LCD_RW = 1;         // 设置为读模式
    LCD_EN = 1;         // 使能信号
    _nop_();            // 短暂延时
    result = (bit)(LCD_DATA & 0x80);  // 读取忙标志位（最高位）
    LCD_EN = 0;         // 关闭使能
    return result;      // 返回忙状态
}

// 向LCD写入命令
void LCD_WriteCmd(unsigned char cmd)
{
    while(LCD_CheckBusy());  // 等待LCD不忙
    
    LCD_RS = 0;         // 选择命令寄存器
    LCD_RW = 0;         // 设置为写模式
    LCD_EN = 0;         // 使能信号置低
    LCD_DATA = cmd;     // 写入命令
    _nop_();            // 短暂延时
    LCD_EN = 1;         // 产生使能脉冲
    _nop_();
    LCD_EN = 0;
    LCD_Delay(5);       // 命令执行延时
}

// 向LCD写入数据
void LCD_WriteData(unsigned char dat)
{
    while(LCD_CheckBusy());  // 等待LCD不忙
    
    LCD_RS = 1;         // 选择数据寄存器
    LCD_RW = 0;         // 设置为写模式
    LCD_EN = 0;         // 使能信号置低
    LCD_DATA = dat;     // 写入数据
    _nop_();            // 短暂延时
    LCD_EN = 1;         // 产生使能脉冲
    _nop_();
    LCD_EN = 0;
    LCD_Delay(5);       // 数据写入延时
}

// LCD1602初始化
void LCD_Init()
{
    LCD_Delay(1500);    // 上电延时，等待LCD稳定
    
    // 初始化序列
    LCD_WriteCmd(0x38); // 设置16x2显示，5x7点阵，8位数据接口
    LCD_WriteCmd(0x0C); // 显示器开，光标关闭，闪烁关闭
    LCD_WriteCmd(0x06); // 文字不动，地址自动+1
    LCD_WriteCmd(0x01); // 清屏
    LCD_Delay(500);     // 清屏延时
}

// 清屏函数
void LCD_Clear()
{
    LCD_WriteCmd(0x01); // 清屏命令
    LCD_Delay(500);     // 清屏需要较长时间
}

// 设置显示位置
void LCD_SetCursor(unsigned char x, unsigned char y)
{
    unsigned char addr;
    
    if (x == 0)
        addr = 0x80 + y;    // 第一行地址从0x80开始
    else
        addr = 0xC0 + y;    // 第二行地址从0xC0开始
        
    LCD_WriteCmd(addr);     // 设置DDRAM地址
}

// 显示字符串
void LCD_ShowString(unsigned char x, unsigned char y, unsigned char *str)
{
    LCD_SetCursor(x, y);    // 设置起始位置
    
    while (*str != '\0')    // 遍历字符串直到结束符
    {
        LCD_WriteData(*str); // 显示当前字符
        str++;              // 指向下一个字符
    }
}

// 显示数字
void LCD_ShowNum(unsigned char x, unsigned char y, unsigned int num, unsigned char len)
{
    unsigned char i;
    unsigned char num_str[6]; // 存储数字字符串
    
    // 将数字转换为字符串
    for (i = 0; i < len; i++)
    {
        num_str[len-1-i] = num % 10 + '0'; // 从个位开始处理
        num /= 10;
    }
    num_str[len] = '\0'; // 字符串结束符
    
    LCD_ShowString(x, y, num_str); // 显示数字字符串
}

// 显示有符号数字
void LCD_ShowSignedNum(unsigned char x, unsigned char y, int num, unsigned char len)
{
    if (num < 0)
    {
        LCD_ShowChar(x, y, '-');    // 显示负号
        num = -num;                 // 取绝对值
        LCD_ShowNum(x, y+1, (unsigned int)num, len-1); // 显示数字部分
    }
    else
    {
        LCD_ShowChar(x, y, '+');    // 显示正号
        LCD_ShowNum(x, y+1, (unsigned int)num, len-1); // 显示数字部分
    }
}

// 显示十六进制数字
void LCD_ShowHexNum(unsigned char x, unsigned char y, unsigned int num, unsigned char len)
{
    unsigned char i, temp;
    unsigned char hex_str[5] = "0x"; // 十六进制前缀
    
    // 转换十六进制数字
    for (i = 0; i < len; i++)
    {
        temp = (num >> ((len-1-i)*4)) & 0x0F; // 提取4位
        if (temp < 10)
            hex_str[2+i] = temp + '0';        // 0-9
        else
            hex_str[2+i] = temp - 10 + 'A';   // A-F
    }
    hex_str[2+len] = '\0'; // 字符串结束符
    
    LCD_ShowString(x, y, hex_str); // 显示十六进制字符串
}

// 显示字符
void LCD_ShowChar(unsigned char x, unsigned char y, unsigned char chr)
{
    LCD_SetCursor(x, y);    // 设置显示位置
    LCD_WriteData(chr);     // 显示字符
}

// 显示浮点数
void LCD_ShowFloat(unsigned char x, unsigned char y, float fnum, unsigned char int_len, unsigned char dec_len)
{
    unsigned int int_part, dec_part;
    float temp;
    
    // 处理整数部分
    if (fnum < 0)
    {
        LCD_ShowChar(x, y, '-');    // 显示负号
        fnum = -fnum;               // 取绝对值
        y++;                        // 列位置后移
    }
    else
    {
        LCD_ShowChar(x, y, '+');    // 显示正号
        y++;                        // 列位置后移
    }
    
    int_part = (unsigned int)fnum;  // 提取整数部分
    LCD_ShowNum(x, y, int_part, int_len); // 显示整数部分
    
    // 显示小数点
    LCD_ShowChar(x, y + int_len, '.');
    
    // 处理小数部分
    temp = fnum - int_part;         // 提取小数部分
    for (dec_len = dec_len; dec_len > 0; dec_len--)
    {
        temp *= 10;                 // 移动小数点
    }
    dec_part = (unsigned int)temp;  // 转换为整数
    
    // 显示小数部分
    LCD_ShowNum(x, y + int_len + 1, dec_part, dec_len);
}