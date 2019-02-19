/*---------------------------------------------------------------------------------------------------------*/
/*                                                                                                         */
/* Copyright(c) 2010 Nuvoton Technology Corp. All rights reserved.                                         */
/*                                                                                                         */
/*---------------------------------------------------------------------------------------------------------*/

#include <stdio.h>

#include "NUC1xx.h"
#include "common.h"
#include "global.h"
#include "FMC.h"
#include "uart.h"

////////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef __USE_DELAY__
void delay(unsigned long ulCNT)
{
	while(ulCNT--);
}

void delay_us(unsigned long ulCNT)
{
	SysTick->LOAD = ulCNT*CyclesPerUs;			// max ulCNT ¡Ö 328000
	SysTick->VAL  = 0;
	SysTick->CTRL = 0x5;						// clk src : core clk, counter enable
	
	while((SysTick->CTRL & (1 << 16)) == 0);    // Waiting for down-count to zero
}

void delay_ms(unsigned long ulCNT)
{
/*
	SysTick->LOAD = ulCNT*CyclesPerUs*1000;		// max ulCNT ¡Ö 328
	SysTick->VAL  = 0;
	SysTick->CTRL = 0x5;						// clk src : core clk, counter enable

	while((SysTick->CTRL & (1 << 16)) == 0);    // Waiting for down-count to zero
*/
	while(ulCNT--)	delay_us(1000);
}
#endif

////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned int DFB_Addr;

void FMC_Enable(void)
{
	SYSCLK->AHBCLK.ISP_EN = 1;
}

void ISP_Enable(void)
{
	FMC->ISPCON.ISPEN = 1;
}

void ISP_Disable(void)
{
	FMC->ISPCON.ISPEN = 0;
}

void ISP_Triger(void)
{
	__set_PRIMASK(1);
	FMC->ISPTRG.ISPGO = 1;    
	__ISB();
	while (FMC->ISPTRG.ISPGO);
	__set_PRIMASK(0);
}

void FMC_Erase(unsigned int addr)
{
	FMC->ISPCMD.FCTRL = 2;		// erase
	FMC->ISPCMD.FCEN = 0;
	FMC->ISPCMD.FOEN = 1;
	FMC->ISPADR = addr;
 	ISP_Triger();
}


void FMC_Write_Byte(unsigned int addr, unsigned int w_data)
{
	ISP_Enable();

	FMC->ISPCMD.FCTRL = 1;		// write
	FMC->ISPCMD.FCEN = 0;
	FMC->ISPCMD.FOEN = 1;
	
	FMC->ISPADR = addr;
	FMC->ISPDAT = w_data;
	ISP_Triger();
	
	ISP_Disable();
}

unsigned char FMC_Read_Byte(unsigned int addr)
{
	union DW_TEMP r_data;
	
	ISP_Enable();

	FMC->ISPCMD.FCTRL = 0;
	FMC->ISPCMD.FCEN = 0;
	FMC->ISPCMD.FOEN = 0;

	FMC->ISPADR = DFB_Addr + addr/4;
    FMC->ISPDAT = 0;
	ISP_Triger();
	r_data.valDW = FMC->ISPDAT;
	
	ISP_Disable();

	return r_data.valB[addr%4];
}

void FMC_Write(unsigned int addr, unsigned int *w_data, unsigned int length)
{
	unsigned int ulCount,flash_addr;

	flash_addr = DFB_Addr + addr;

	ISP_Enable();
 	
 	FMC_Erase(flash_addr);
//-----------------------------------------------------
	FMC->ISPCMD.FCTRL = 1;		// write
	FMC->ISPCMD.FCEN = 0;
	FMC->ISPCMD.FOEN = 1;
	if(addr == FMC_PAGE1 || addr == FMC_PAGE2)
	{
		for(ulCount=0;ulCount<length;ulCount+=1) 
		{
			FMC->ISPADR = flash_addr;
			FMC->ISPDAT = *(w_data++);

			ISP_Triger();
			flash_addr += 4;
		}
	}
	else
	{
		for(ulCount=0;ulCount<length;ulCount+=4) 
		{
			FMC->ISPADR = flash_addr;
			FMC->ISPDAT = *(w_data++);
			ISP_Triger();
			flash_addr += 4;
		}
	}
	ISP_Disable();
}

void FMC_Read(unsigned int addr, unsigned int *r_data, unsigned int length)
{
	unsigned int ulCount,flash_addr;
	
	flash_addr = DFB_Addr + addr;

	ISP_Enable();

	FMC->ISPCMD.FCTRL = 0;
	FMC->ISPCMD.FCEN = 0;
	FMC->ISPCMD.FOEN = 0;
    FMC->ISPDAT = 0;
	
	if(addr == FMC_PAGE1 || addr == FMC_PAGE2)
	{
		for(ulCount=0;ulCount<length;ulCount+=1) 
		{
			FMC->ISPADR = flash_addr;
			ISP_Triger();
			*(r_data++) = FMC->ISPDAT;
			flash_addr += 4;
		}
	}
	else
	{
		for(ulCount=0;ulCount<length;ulCount+=4) 
		{
			FMC->ISPADR = flash_addr;
			ISP_Triger();
			*(r_data++) = FMC->ISPDAT;
			flash_addr += 4;
		}
	}
	ISP_Disable();
}

void FMC_WriteConfig(unsigned int cfg_data0, unsigned int cfg_data1)
{       
	ISP_Enable();

	FMC->ISPCON.CFGUEN = 1;

	FMC_Erase(CONFIG0);

	FMC->ISPCMD.FCTRL = 1;		// write
	FMC->ISPCMD.FCEN = 0;
	FMC->ISPCMD.FOEN = 1;

	FMC->ISPADR = CONFIG0;
	FMC->ISPDAT = cfg_data0;
	ISP_Triger();

	FMC->ISPADR = CONFIG1;
	FMC->ISPDAT = cfg_data1;
	ISP_Triger();

	FMC->ISPCON.CFGUEN = 0;

	ISP_Disable();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef __USE_WDT__
/*	
WDT timeout Interval : time = (2^(4+(2*intvl))*clk ( + (2^10)*clk)
clk src:int 10k osc -> clk = 0.1ms
0: 2^4  * 0.1 =      1.6ms + 102.4ms =    104.0ms
1: 2^6  * 0.1 =      6.4ms			 =    108.8ms
2: 2^8  * 0.1 =     25.6ms			 =    128.0ms
3: 2^10 * 0.1 =    102.4ms			 =    204.8ms
4: 2^12 * 0.1 =    409.6ms			 =    512.0ms
5: 2^14 * 0.1 =  1,638.4ms			 =  1,740.0ms
6: 2^16 * 0.1 =  6,553.6ms			 =  6,656.0ms
7: 2^18 * 0.1 = 26,214.4ms			 = 26,316.8ms

clk src:HCLK/2048 -> clk = 1/24414 = ¡Ö 41us
0: 2^4  * 0.041 =      0.656ms + 102.4ms =    103.056ms
1: 2^6  * 0.041 =      2.624ms			 =    105.024ms
2: 2^8  * 0.041 =     10.496ms			 =    112.896ms
3: 2^10 * 0.041 =     41.984ms			 =    144.384ms
4: 2^12 * 0.041 =    167.936ms			 =    270.336ms
5: 2^14 * 0.041 =    671.744ms			 =    774.144ms
6: 2^16 * 0.041 =  2,686.976ms			 =  2,789.376ms
7: 2^18 * 0.041 = 10,747.904ms			 = 10,850.304ms
*/
void wdt_enable(E_WDT_INTERVAL interval)
{
	SYSCLK->CLKSEL1.WDT_S = 2;   
	SYSCLK->APBCLK.WDT_EN = 1;		// Enable WatchDog Timer Clock
	WDT->WTCR.WTIS = interval;		// Select WatchDog Timer Interval
	WDT->WTCR.WTRE = 1;				// Enable Watchdog Timer Reset function
	WDT->WTCR.WTE = 1;				// Enable WDT
	WDT->WTCR.WTR = 1;				// Reset WDT counter
	WDT->WTCR.WTIF = 1;				// clear wdt reset flag bit
}

void wdt_reset(void)
{
	WDT->WTCR.WTR = 1;			// Reset WDT counter
}
#endif
////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned int abs_diff(unsigned int A, unsigned int B)
{
	unsigned int result;

	if(A>B)	result = A - B;
	else	result = B - A;

	return result;
}

unsigned char ascii_to_hex(unsigned char x, unsigned char y)
{
	unsigned char tmp;

	if(x<0x3a)		tmp = (x-0x30)<<4;
	else if(x<0x47) tmp = (x-0x37)<<4;
	else			tmp = (x-0x57)<<4;

	if(y<0x3a)		tmp |= (y-0x30);
	else if(y<0x47)	tmp |= (y-0x37);
	else			tmp |= (y-0x57);

	return tmp;
}

unsigned char ascii_to_dec(unsigned char x, unsigned char y)
{
	unsigned char tmp;

	if(x<0x3a)		tmp = (x-0x30)*10;
	else if(x<0x47) tmp = (x-0x37)*10;
	else			tmp = (x-0x57)*10;
	if(y<0x3a)		tmp += (y-0x30);
	else if(y<0x47)	tmp += (y-0x37);
	else			tmp += (y-0x57);

	return tmp;
}

void hex_to_ascii(unsigned char x, char *y)
{
	y[0] = ((x>>4)<10)?((x>>4)+0x30):((x>>4)+0x37);
	y[1] = ((x&0x0f)<10)?((x&0x0f)+0x30):((x&0x0f)+0x37);
}

void dec_to_ascii(unsigned char x, char *y, unsigned char len)
{
	if(len == 3){
		y[0] = ((x/100)+0x30);
		y[1] = (((x%100)/10)+0x30);
		y[2] = ((x%10)+0x30);
	}
	else if(len == 2){
		y[0] = ((x/10)|0x30);
		y[1] = ((x%10)|0x30);
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef __USE_VALUE_CTRL__
void value_updn(unsigned int change_value, unsigned char *value, unsigned int lo, unsigned int hi)
{
	if(change_value){
		if(*value >= hi)	*value = lo;
		else	(*value)++;
	}
	else{
		if(*value == lo)	*value = hi;
		else	(*value)--;
	}
}
#endif
////////////////////////////////////////////////////////////////////////////////////////////////////
