#ifndef __PCM1801_H
#define __PCM1801_H

#include "STC12C5A60S2.H"
#include "tpyedef.h"

#define FOSC  32768000L
#define T128KHz (FOSC /4 /8000)

#define SPIF        0x80        //SPSTAT.7
#define WCOL        0x40        //SPSTAT.6

#define SSIG        0x80        //SPCTL.7
#define SPEN        0x40        //SPCTL.6
#define DORD        0x20        //SPCTL.5
#define MSTR        0x10        //SPCTL.4
#define CPOL        0x08        //SPCTL.3
#define CPHA        0x04        //SPCTL.2
#define SPDHH       0x00        //CPU_CLK/4
#define SPDH        0x01        //CPU_CLK/16
#define SPDL        0x02        //CPU_CLK/64
#define SPDLL       0x03        //CPU_CLK/128
#define ESPI        0x02        //IE2.1

#define MASTER

sbit SPISS = P1 ^ 3;       //SPI slave select, connect to slave' SS(P1.4) pin
sbit L3 = P3 ^ 4;
//sbit T0CLKO=P3^4;
sbit T1CLKO = P3 ^ 5;
sbit dat = P1 ^ 0;

extern uint8_t pc_request_voice_data;


void InitSPI();
void Init_Timer();
void Init_pca();
extern void PCM1801_Init();



#endif