#include "timer.h"

void Init_Timer0(void)
{
	TMOD |= 0x01;	  //使用模式1，16位定时器，使用"|"符号可以在使用多个定时器时不受影响	
	TH0 = (65536 - TIMER50MS) >> 8; //重新赋值 5ms
	TL0 = (65536 - TIMER50MS) & 0xff;
	EA = 1;            //总中断打开
	ET0 = 1;           //定时器中断打开
	TR0 = 1;           //定时器开关打开
}

void Timer0_isr(void) interrupt 1  //定时器0中断服务程序
{
	TF0 = 0;
	TH0 = (65536 - TIMER50MS) >> 8;		  //重新赋值 50ms
	TL0 = (65536 - TIMER50MS) & 0xff;
}