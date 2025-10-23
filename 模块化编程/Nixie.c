#include <reg51.h>
#include "Delay.h"
sbit P2_2 = P2^2;
sbit P2_3 = P2^3;
sbit P2_4 = P2^4;

unsigned char NixieTable[] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F};

void Nixie(unsigned char Location, unsigned char Number)
{
    P0 = 0x00;  // 消影
    
    switch(Location)
    {
        case 1: P2_4 = 1; P2_3 = 1; P2_2 = 1; break;
        case 2: P2_4 = 1; P2_3 = 1; P2_2 = 0; break;
        case 3: P2_4 = 1; P2_3 = 0; P2_2 = 1; break;  
        case 4: P2_4 = 1; P2_3 = 0; P2_2 = 0; break;
        case 5: P2_4 = 0; P2_3 = 1; P2_2 = 1; break;
        case 6: P2_4 = 0; P2_3 = 1; P2_2 = 0; break;
        case 7: P2_4 = 0; P2_3 = 0; P2_2 = 1; break;
        case 8: P2_4 = 0; P2_3 = 0; P2_2 = 0; break;
    }
    
    P0 = NixieTable[Number];
    Delay(2);  // 每个数码管显示约2ms
}
