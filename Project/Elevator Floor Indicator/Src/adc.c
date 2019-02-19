
#include <stdio.h>

#include "NUC1xx.h"
#include "DrvSYS.h"
#include "DrvGPIO.h"

#include "adc.h"

void ADC_Init(void)
{
	SYS->IPRSTC2.ADC_RST = 1;					// reset ADC
	SYS->IPRSTC2.ADC_RST = 0;
	
	SYSCLK->CLKSEL1.ADC_S = EXTERNAL_12MHZ;		// ADC clock source
	SYSCLK->CLKDIV.ADC_N = 4;					// Set ADC divisor
	SYSCLK->APBCLK.ADC_EN = 1;					// ADC engine clock enable

	ADC->ADCR.ADEN = 1;							// ADC enable
	ADC->ADCR.DIFFEN = 0;						// single end input
	ADC->ADCR.ADMD = 0;							// single mode
	ADC->ADCHER.CHEN = 0;
}

void ADC_En_Ch(unsigned char ch)
{
	DrvGPIO_DisableDigitalInputBit(E_GPA, ch);	// Disable the PA.x digital input path
	
	switch(ch){									// Configure PA.x as ADC analog input pin
		case 0:	DrvGPIO_InitFunction(E_FUNC_ADC0);	break;
		case 1:	DrvGPIO_InitFunction(E_FUNC_ADC1);	break;
		case 2:	DrvGPIO_InitFunction(E_FUNC_ADC2);	break;
		case 3:	DrvGPIO_InitFunction(E_FUNC_ADC3);	break;
		case 4:	DrvGPIO_InitFunction(E_FUNC_ADC4);	break;
		case 5:	DrvGPIO_InitFunction(E_FUNC_ADC5);	break;
		case 6:	DrvGPIO_InitFunction(E_FUNC_ADC6);	break;
		case 7:	DrvGPIO_InitFunction(E_FUNC_ADC7);	break;
	}
}

unsigned int Get_ADC(unsigned char ch)
{
	unsigned int i, ad_value = 0;

	ADC->ADCHER.CHEN = 1<<ch;					// Set the ADC channel

	for(i=0;i<4;i++){
		ADC->ADCR.ADST = 1;						// Start A/D convert
		while(ADC->ADSR.ADF==0);				// Wait  A/D Conversion
		ADC->ADSR.ADF = 1;						// Clear A/D Conversion End Flag
		ad_value += ADC->ADDR[ch].RSLT&0xFFF;
	}
	
	return (ad_value/4);
}
