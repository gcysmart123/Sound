#include "wch372.h"
#include "pcm1801.h"

unsigned char length, i;//定义数据长度和数据位数//
unsigned char IntStaus;//定义中断状态//
unsigned char buffer[64];//数据数组//

extern unsigned char re_send_status_to_pc;

/*------------------------------------------------
中断服务程序
------------------------------------------------*/
void wch372_interrupt()  interrupt 0
{

	ch372_wr_cmd_port(CMD_GET_STATUS);//获取中断状态//
	IntStaus = ch372_rd_dat_port();//读取芯片中断状态//
	CH372_INT = 1;
	switch (IntStaus)
	{
		case USB_INT_EP2_OUT://usb输出数据//
			ch372_wr_cmd_port(CMD_RD_USB_DATA);//单片机向ch372写入读取usb接口数据的命令//
			length = ch372_rd_dat_port();//获取后续数据长度//
			for(i = 0; i < length; i++)
			{
				buffer[i] = ch372_rd_dat_port();//按照获得的数据位数，将数据存入buffer[i]//
			}		
			if (length == 7) /*the length of "command for check wire status",and simply use the length for re send the status to PC */
			{
				if (buffer[3] == 0x09)
				{
					re_send_status_to_pc = 1;
				}
				
			}
			if (length == 8)/*the length of "command for start to record the voice",and simply use the length for the flag of start record */
			{
				if (buffer[3] == 0x10)
				{
					if (buffer[5] == 0x00)
					{
						pc_request_voice_data = 0;
					}
					if (buffer[5] == 0x01)
					{
						pc_request_voice_data = 1;
					}
				}		
			}

			//ch372_wr_cmd_port(CMD_WR_USB_DATA5);//单片机向ch372写入向usb写入数据的命令//
			//ch372_wr_dat_port(length);//告诉pc数据长度//
			//for(i = 0; i<length; i++)
			//{
			//	ch372_wr_dat_port(buffer[i]);
			//}

		//pc机验证数据的正确性//
		//	 wch372_send1byte(0x09,0x01);
		//s_linenum_p = 1;
			delay50ms();

		ch372_wr_cmd_port(CMD_UNLOCK_USB);
		break;
	case USB_INT_EP2_IN://usb输入数据//
		ch372_wr_cmd_port(CMD_UNLOCK_USB);//释放usb缓冲区//
		break;
	default:
		ch372_wr_cmd_port(CMD_UNLOCK_USB);//释放usb缓冲区//
		break;
	}
}


/*------------------------------------------------
CH372初始化子函数
------------------------------------------------*/
void ch372_init()
{

	unsigned char i;/* ??CH375??????,????,????? */
	P4SW |= 0x10; //设置P4.4为普通IO口
	delay50ms();

	ch372_wr_cmd_port(CMD_CHECK_EXIST);  /* ??CH375?????? */
	ch372_wr_dat_port(0x55);	/* ?????? */
	i = ~0x55;  /* ????????????? */
	if (ch372_rd_dat_port() != i)
	{
		for (i = 80; i != 0; i--)
		{
			ch372_wr_cmd_port(CMD_RESET_ALL);  /* ???????,?????? */
			ch372_rd_dat_port();
		}
		ch372_wr_cmd_port(0);
		delay50ms();  /* ??50ms */
	}
	ch372_wr_cmd_port(CMD_SET_USB_MODE);
	ch372_wr_dat_port(2);  /* ??????????USB???? */
	for (i = 100; i != 0; i--)
	{
		if (ch372_rd_dat_port() == CMD_RET_SUCCESS) break;
	}

	IT0 = 0;  /* ??????????? */
	IE0 = 0;  /* ????? */
	EX0 = 1;  /* ??CH372?? */
	EA = 1;  /* 允许中断 */
}


/*------------------------------------------------
CH372写命令子函数
------------------------------------------------*/
void ch372_wr_cmd_port(unsigned char cmd)   /* 向CH375的命令端口写入命令,周期不小于4uS,如果单片机较快则延时 */
{
	P2M1 = 0x00;
	P2M0 = 0xff;
	CH372_A0 = 0;
	CH372_WR = 1;
	CH372_RD = 1;
	CH372_CS = 1;


	CH372_A0 = 1;
	CH372_RD = 1;
	CH372_WR = 0;
	CH372_CS = 0;
	CH372_CMD_PORT = cmd;




	CH372_WR = 1;
	CH372_RD = 1;
	CH372_A0 = 0;
	CH372_CS = 1;



}
/*------------------------------------------------
CH372写入数据子函数
------------------------------------------------*/

void ch372_wr_dat_port(unsigned char dat)
{
	P2M1 = 0x00;
	P2M0 = 0xff;
	CH372_A0 = 0;
	CH372_WR = 1;
	CH372_RD = 1;
	CH372_CS = 1;


	CH372_CMD_PORT = dat;
	CH372_RD = 1;
	CH372_WR = 0;
	CH372_CS = 0;


	CH372_WR = 1;
	CH372_RD = 1;
	CH372_A0 = 0;
	CH372_CS = 1;
}
/*------------------------------------------------
CH372读取数据子函数
------------------------------------------------*/

unsigned char ch372_rd_dat_port(void)
{
	unsigned char dat;

	P2M1 = 0xff;
	P2M0 = 0x00;

	CH372_A0 = 0;
	CH372_WR = 1;
	CH372_RD = 1;
	CH372_CS = 1;


	CH372_WR = 1;
	CH372_RD = 0;
	CH372_CS = 0;
	dat = CH372_CMD_PORT;




	CH372_WR = 1;
	CH372_RD = 1;
	CH372_A0 = 0;
	CH372_CS = 1;
	return(dat);
}

void wch372_send1byte(unsigned char function, unsigned char uesrdata)
{
	if (pc_request_voice_data == 0)
	{
		ch372_wr_cmd_port(CMD_WR_USB_DATA5);//单片机向ch372写入向usb写入数据的命令//
		ch372_wr_dat_port(7);//告诉pc数据长度//
		ch372_wr_dat_port(0x55);//pc机验证数据的正确性//
		ch372_wr_dat_port(0x02);
		ch372_wr_dat_port(0x55);
		ch372_wr_dat_port(function);
		ch372_wr_dat_port(uesrdata);
		ch372_wr_dat_port(function + uesrdata);
		ch372_wr_dat_port(0x16);
	}
}

void wch372_send2byte(unsigned char function, unsigned char uesrdata1, unsigned char uesrdata2)
{
	if (pc_request_voice_data == 0)
	{
		ch372_wr_cmd_port(CMD_WR_USB_DATA5);//单片机向ch372写入向usb写入数据的命令//
		ch372_wr_dat_port(8);//告诉pc数据长度//
		ch372_wr_dat_port(0x55);//pc机验证数据的正确性//
		ch372_wr_dat_port(0x03);
		ch372_wr_dat_port(0x55);
		ch372_wr_dat_port(function);
		ch372_wr_dat_port(uesrdata1);
		ch372_wr_dat_port(uesrdata2);
		ch372_wr_dat_port(function + uesrdata1 + uesrdata2);
		ch372_wr_dat_port(0x16);
	}
}

void delay50ms()
{
	unsigned char a, b, c;
	for (c = 123; c>0; c--)
	for (b = 116; b>0; b--)
	for (a = 9; a>0; a--);
}