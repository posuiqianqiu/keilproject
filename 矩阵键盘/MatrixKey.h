#ifndef MATRIXKEY_H
#define MATRIXKEY_H

#include <reg52.h>

// 矩阵键盘引脚定义 - 使用P1口
// 行线：P1.7-P1.4（高4位，输入）
// 列线：P1.3-P1.0（低4位，输出）
#define KEY_PORT P1

// 函数声明

/**
 * @brief 矩阵键盘初始化
 * @param 无
 * @return 无
 */
void MatrixKey_Init(void);

/**
 * @brief 列扫描法检测按键
 * @param 无
 * @return 按键值 (1-16) 或 0(无按键)
 * @说明 采用列扫描法，检测按键松开时的状态
 */
unsigned char MatrixKey_ColumnScan(void);

/**
 * @brief 获取按键并处理显示
 * @param 无
 * @return 无
 * @说明 主功能函数，按下无现象，松手显示对应编号
 */
void MatrixKey_ProcessKey(void);

/**
 * @brief 获取要显示的按键值
 * @param 无
 * @return 要显示的按键编号 (0-16)
 * @说明 返回0表示无显示，1-16表示对应按键编号
 */
unsigned char MatrixKey_GetDisplayValue(void);

#endif