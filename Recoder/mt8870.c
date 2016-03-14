#include "mt8870.h"
#include "wch372.h"
#include "pcm1801.h"

void mt8870_init(void)
{
	IT1 = 1;  /* ??????????? */
	IE1 = 0;  /* ????? */
	EX1 = 1;  /* ??CH372?? 外部中断初始化*/
	P0M1 = 0X0F; //P0端口设置为输入
	P0M0 = 0X00;
}

void exint1() interrupt 2           //(location at 0013H)
{
	unsigned int dtmf_table = 0;
	dtmf_table = P0;
	dtmf_table &= 0x0f;	
	pc_request_voice_data = 0;
	switch (dtmf_table)
	{
		case 1: wch372_send2byte(0xA2, 0x01, 0x31); break;
		case 2: wch372_send2byte(0xA2, 0x01, 0x32); break;
		case 3: wch372_send2byte(0xA2, 0x01, 0x33); break;
		case 4: wch372_send2byte(0xA2, 0x01, 0x34); break;
		case 5: wch372_send2byte(0xA2, 0x01, 0x35); break;
		case 6: wch372_send2byte(0xA2, 0x01, 0x36); break;
		case 7: wch372_send2byte(0xA2, 0x01, 0x37); break;
		case 8: wch372_send2byte(0xA2, 0x01, 0x38); break;
		case 9: wch372_send2byte(0xA2, 0x01, 0x39); break;
		case 10: wch372_send2byte(0xA2, 0x01, 0x30); break;
		//case 11: wch372_send2byte(0xA2, 0x01, 0x0b); break;
		//case 12: wch372_send2byte(0xA2, 0x01, 0x0c); break;
		//case 13: wch372_send2byte(0xA2, 0x01, 0x00); break;
		//case 14: wch372_send2byte(0xA2, 0x01, 0x00); break;
		//case 15: wch372_send2byte(0xA2, 0x01, 0x00); break;
		default: break;
	}
	pc_request_voice_data = 1;
}