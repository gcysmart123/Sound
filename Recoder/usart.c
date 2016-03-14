#include "usart.h"
#include "wch372.h"

bit busy;
unsigned char ht9032c_table = 0;
unsigned char ht9302_i = 0;
unsigned char ht9032_number_length = 0;
unsigned char ht9032_data[50] = { 0 };

/*----------------------------
UART init
----------------------------*/
void usart_init(void)
{
//#if (PARITYBIT == NONE_PARITY)
//	SCON = 0x50;            //8-bit variable UART
//#elif (PARITYBIT == ODD_PARITY) || (PARITYBIT == EVEN_PARITY) || (PARITYBIT == MARK_PARITY)
//	SCON = 0xda;            //9-bit variable UART, parity bit initial to 1
//#elif (PARITYBIT == SPACE_PARITY)
//	SCON = 0xd2;            //9-bit variable UART, parity bit initial to 0
//#endif
//
//
//
//	AUXR |= 0x40;                //???1?1T??
//	TMOD = 0x20;            //Set Timer1 as 8-bit auto reload mode
//	TH1 = TL1 = -(FOSC / 32 / BAUD); //Set auto-reload vaule
//	TR1 = 1;                //Timer1 start run
//	ES = 1;                 //Enable UART interrupt

#if (PARITYBIT == NONE_PARITY)
	S2CON = 0x50;           //8-bit variable UART
#elif (PARITYBIT == ODD_PARITY) || (PARITYBIT == EVEN_PARITY) || (PARITYBIT == MARK_PARITY)
	S2CON = 0xda;           //9-bit variable UART, parity bit initial to 1
#elif (PARITYBIT == SPACE_PARITY)
	S2CON = 0xd2;           //9-bit variable UART, parity bit initial to 0
#endif

	BRT = -(FOSC / 32 / 12 / BAUD);  //Set auto-reload vaule of baudrate generator
	AUXR |= 0x10;            //Baudrate generator work in 1T mode
	IE2 = 0x01;             //Enable UART2 interrupt
	EA = 1;                 //Open master interrupt switch

	uart_printf("STC12C5A60S2\r\nUart2 Test !\r\n");
}


///*----------------------------
//UART interrupt service routine
//----------------------------*/
//void Uart_Isr() interrupt 4 using 1
//{
//	if (RI)
//	{
//		RI = 0;             //Clear receive interrupt flag
//		//        P0 = SBUF;          //P0 show UART data
//		//        bit9 = RB8;         //P2.2 show parity bit
//	}
//	if (TI)
//	{
//		TI = 0;             //Clear transmit interrupt flag
//		busy = 0;           //Clear transmit busy flag
//	}
//}

void SendData(BYTE dat)
{
	while (busy);           //Wait for the completion of the previous data is sent
	ACC = dat;              //Calculate the even parity bit P (PSW.0)
	if (P)                  //Set the parity bit according to P
	{
#if (PARITYBIT == ODD_PARITY)
		S2CON &= ~S2TB8;    //Set parity bit to 0
#elif (PARITYBIT == EVEN_PARITY)
		S2CON |= S2TB8;     //Set parity bit to 1
#endif
	}
	else
	{
#if (PARITYBIT == ODD_PARITY)
		S2CON |= S2TB8;     //Set parity bit to 1
#elif (PARITYBIT == EVEN_PARITY)
		S2CON &= ~S2TB8;    //Set parity bit to 0
#endif
	}
	busy = 1;
	S2BUF = ACC;            //Send data to UART2 buffer
}

/*----------------------------
Send a string to UART
Input: s (address of string)
Output:None
----------------------------*/
void SendString(char *s)
{
	while (*s)              //Check the end of the string
	{
		SendData(*s++);     //Send current char and increment string ptr
	}
}

void uart_printf(const char *fmt, ...)
{
	va_list ap;
	char xdata string[512];//??????RAM,?????RAM,????????RAM??(???1024)  

	va_start(ap, fmt);
	vsprintf(string, fmt, ap);//???????sprintf??,?????,??????,????  
	SendString(string);
	va_end(ap);
}


void Uart2() interrupt 8 using 1
{
	unsigned char temp = 0, j = 0;

	if (S2CON & S2RI)
	{
		S2CON &= ~S2RI;     //Clear receive interrupt flag
		//        P0 = S2BUF;         //P0 show UART data
		//        P2 = (S2CON & S2RB8);//P2.2 show parity bit
		temp = S2BUF;
		//SendData(temp);
		/******/


		if (temp == 0x80)
		{
			ht9032c_table = 1;
			return;
		}
		if (ht9032c_table == 1)
		{
			if (temp == 0x02)
			{
				ht9032c_table = 2;
			}
			return;
		}
		if (ht9032c_table == 2)
		{
			ht9032_number_length = temp;
			ht9032c_table = 3;
			return;
		}
		if (ht9032c_table == 3)
		{
			if (ht9032_number_length>0)
			{
				ht9032_data[ht9032_number_length] = temp;

			}
			ht9032_number_length--;
			if (ht9032_number_length == 0)
			{
				ht9032c_table = 0;
				//wch372_send2byte(0xA2,0x01,temp);
				//					ch372_wr_cmd_port(CMD_WR_USB_DATA7);//单片机向ch372写入向usb写入数据的命令//
				//  	      ch372_wr_dat_port(11);//告诉pc数据长度//

				ch372_wr_cmd_port(CMD_WR_USB_DATA5);//单片机向ch372写入向usb写入数据的命令//
				ch372_wr_dat_port(8);//告诉pc数据长度//
				ch372_wr_dat_port(0xAA);
				ch372_wr_dat_port(0X01);
				for (j = 11; j>5; j--)
				{
					ch372_wr_dat_port(ht9032_data[j]);
					//wch372_send2byte(0xA2,0x01,ht9032_data[j]);
				}//pc机验证数据的正确性//
				delay1ms();
				ch372_wr_cmd_port(CMD_WR_USB_DATA5);//单片机向ch372写入向usb写入数据的命令//
				ch372_wr_dat_port(8);//告诉pc数据长度//
				ch372_wr_dat_port(0xAA);
				ch372_wr_dat_port(0X01);
				for (j = 5; j>0; j--)
				{
					ch372_wr_dat_port(ht9032_data[j]);
					//wch372_send2byte(0xA2,0x01,ht9032_data[j]);
				}//pc机验证数据的正确性//
				ch372_wr_dat_port(0xAA);
			}
		}




	}
	if (S2CON & S2TI)
	{
		S2CON &= ~S2TI;     //Clear transmit interrupt flag
		busy = 0;           //Clear transmit busy flag
	}
}

void delay1ms(void)   //误差 0us
{
	unsigned char a, b, c;
	for (c = 1; c>0; c--)
	for (b = 38; b>0; b--)
	for (a = 214; a>0; a--);
}
