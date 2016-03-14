#include "STC12C5A60S2.H"
#include "usart.h"
#include "adc.h"
#include "wch372.h"
#include "pcm1801.h"
#include "mt8870.h"

void main(void)
{
	unsigned char adc_value = 0;

	usart_init();
	InitADC();
	ch372_init();
	PCM1801_Init();
	mt8870_init();
	
	while (1)
	{
		tel_state_process();
		//adc_value = GetADCResult_av(0);
		//uart_printf("ADC value is %d!\r\n", (unsigned int)adc_value);
	}
}