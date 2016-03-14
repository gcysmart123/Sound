#include "timer.h"

void Init_Timer0(void)
{
	TMOD |= 0x01;	  //ʹ��ģʽ1��16λ��ʱ����ʹ��"|"���ſ�����ʹ�ö����ʱ��ʱ����Ӱ��	
	TH0 = (65536 - TIMER50MS) >> 8; //���¸�ֵ 5ms
	TL0 = (65536 - TIMER50MS) & 0xff;
	EA = 1;            //���жϴ�
	ET0 = 1;           //��ʱ���жϴ�
	TR0 = 1;           //��ʱ�����ش�
}

void Timer0_isr(void) interrupt 1  //��ʱ��0�жϷ������
{
	TF0 = 0;
	TH0 = (65536 - TIMER50MS) >> 8;		  //���¸�ֵ 50ms
	TL0 = (65536 - TIMER50MS) & 0xff;
}