#include <reg51.h>
#include "Delay.h"
#include "Nixie.h"


void main()
{
	while(1)
	{
	  Nixie(1, 1);  // 第1个数码管显示1
		Nixie(2, 2);  // 第2个数码管显示2
		Nixie(3, 3);  // 第3个数码管显示3
		Nixie(4, 4);  // 第4个数码管显示4
		Nixie(5, 5);  // 第5个数码管显示5
		Nixie(6, 6);  // 第6个数码管显示6
		Nixie(7, 7);  // 第7个数码管显示7
		Nixie(8, 8);  // 第8个数码管显示8
	}
}