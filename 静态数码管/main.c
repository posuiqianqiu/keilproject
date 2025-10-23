#include <reg51.h>

// 定义P2口的各个位
sbit P2_0 = P2^0;
sbit P2_1 = P2^1;
sbit P2_2 = P2^2;
sbit P2_3 = P2^3;
sbit P2_4 = P2^4;
sbit P2_5 = P2^5;
sbit P2_6 = P2^6;
sbit P2_7 = P2^7;

unsigned char NixieTable[] = {0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F};

void Delay(unsigned int t)  // 简单延时函数
{
    while(t--);
}

void Nixie(unsigned char Location, unsigned char Number)
{
    // 先关闭段选，消影
    P0 = 0x00;
    
    switch(Location)
    {
        case 1: P2_4 = 1; P2_3 = 1; P2_2 = 1; break; // Y7
        case 2: P2_4 = 1; P2_3 = 1; P2_2 = 0; break; // Y6
        case 3: P2_4 = 1; P2_3 = 0; P2_2 = 1; break; // Y5  
        case 4: P2_4 = 1; P2_3 = 0; P2_2 = 0; break; // Y4 ← 5号LED
        case 5: P2_4 = 0; P2_3 = 1; P2_2 = 1; break; // Y3
        case 6: P2_4 = 0; P2_3 = 1; P2_2 = 0; break; // Y2
        case 7: P2_4 = 0; P2_3 = 0; P2_2 = 1; break; // Y1
        case 8: P2_4 = 0; P2_3 = 0; P2_2 = 0; break; // Y0
    }
    
    P0 = NixieTable[Number];  // 输出段码
    Delay(1000);  // 延时显示
}

void main()
{
    while(1)
    {
        Nixie(6, 5);  // 在5号LED（对应case 4）显示数字3
    }
}