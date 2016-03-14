#include "pcm1801.h"
#include "wch372.h"
#include "usart.h"

uint16_t val = 0;
uint16_t value = 0;
uint8_t spi_c = 0;

uint8_t pc_request_voice_data = 0;

void spi_isr() interrupt 9 using 1     //SPI interrupt routine 9 (004BH)
{
	SPSTAT = SPIF | WCOL;       //clear SPI status

#ifdef MASTER
	// SPISS = 1;                  //push high slave SS
	if (spi_c == 0)	// 第一次读取数据
	{
		value |= SPDAT << 8;
	}
#else                           //for salve (receive SPI data from master and
	SPDAT = SPDAT;              //           send previous SPI data to master)
#endif

	if (spi_c == 1)
	{
		value |= SPDAT;
		SPDAT = 0x55;
		spi_c = 0;	
		if (pc_request_voice_data == 1)
		{
			ch372_wr_cmd_port(CMD_WR_USB_DATA7);//单片机向ch372写入向usb写入数据的命令//
			ch372_wr_dat_port(2);//告诉pc数据长度//
			ch372_wr_dat_port(value >> 8);//pc机验证数据的正确性//
			ch372_wr_dat_port(value);
		}
	}
}


void Init_Timer()
{
	TMOD |= 0x20;						 //???0,1???2
	//AUXR |= 0x80;					 //
	AUXR |= 0x40;
	//TH0 = 245;
	TH1 = 252;

	WAKE_CLKO = WAKE_CLKO | 0X02;		 //
	EA = 1;			//?????

	//???????
	TR1 = 1;
	//ET0=1;
}

void Init_pca()
{
	CMOD = 0x02;			   //   ??????????PCA FOSC / 2
	//   ??PCA???????
	CCON = 0x00;	     		   //	 ??PCA?????
	//	 PCA???????
	//	 ??CF???
	//    ??????????
	// 	 ??PCA???
	CL = 0;
	CH = 0;
	CCAPM0 = 0x49;			// 	PCA??0???16??????
	val = T128KHz;
	CCAP0L = val;
	CCAP0H = val >> 8;		//   ??PCA??0
	val += T128KHz;
	EA = 1;
	CR = 1;					// 	PCA???????
}

void PCA_isr() interrupt 7 using 1
{
	
	CCF0 = 0x00;				            //   ??????
	CCAP0L = val;
	CCAP0H = val >> 8;		//   ??PCA??0
	val += T128KHz;
	L3 = ~L3;

	// SPISS = 0; 
	//pull low slave SS
	//		if(line_up ==1)
	{
		spi_c = 1;
		value = 0;
		SPDAT = 0x55;                //trigger SPI send
	}
	// SPISS = 1;                  //push high slave SS
}

void InitSPI()
{
	SPDAT = 0;                  //initial SPI data
	SPSTAT = SPIF | WCOL;       //clear SPI status
#ifdef MASTER
	SPCTL = SPEN | MSTR | SPDH | CPOL | CPHA;        //master mode
#else
	SPCTL = SPEN;               //slave mode
#endif
}

void PCM1801_Init()
{
	Init_Timer();
	Init_pca();
	InitSPI();
	IE2 |= ESPI;
}

