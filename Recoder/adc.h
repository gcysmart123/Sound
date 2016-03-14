#ifndef __ADC_H
#define __ADC_H

#include "STC12C5A60S2.H"
#include "intrins.h"
#include "tpyedef.h"


#define ADC_POWER   0x80            //ADC power control bit
#define ADC_FLAG    0x10            //ADC complete flag
#define ADC_START   0x08            //ADC start control bit
#define ADC_SPEEDLL 0x00            //420 clocks
#define ADC_SPEEDL  0x20            //280 clocks
#define ADC_SPEEDH  0x40            //140 clocks
#define ADC_SPEEDHH 0x60            //70 clocks


extern void InitADC();
unsigned char GetADCResult(BYTE ch);
void Delay(WORD n);
unsigned char GetADCResult_av(BYTE ch);
extern void tel_state_process(void);

#endif