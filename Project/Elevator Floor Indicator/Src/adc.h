/*-----------------------------------------------------------------------------------------------*/
/*                                                                                               */
/* Copyright(c) 2010 Nuvoton Technology Corp. All rights reserved.                               */
/*                                                                                               */
/*-----------------------------------------------------------------------------------------------*/
#ifndef __ADC_H__
#define __ADC_H__

typedef enum {EXTERNAL_12MHZ=0, INTERNAL_PLL=1, INTERNAL_HCLK=2, INTERNAL_RC22MHZ=3} E_ADC_CLK_SRC;

void ADC_Init(void);
void ADC_En_Ch(unsigned char ch);
unsigned int Get_ADC(unsigned char ch);

#endif
