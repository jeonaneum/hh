/*---------------------------------------------------------------------------------------------------------*/
/*                                                                                                         */
/* Copyright(c) 2010 Nuvoton Technology Corp. All rights reserved.                                         */
/*                                                                                                         */
/*---------------------------------------------------------------------------------------------------------*/

#ifndef __COMMON_H__
#define __COMMON_H__

//#define __XTAL      (12000000UL)
//#define __XTAL      (12288000UL)
//#define __XTAL      (7372800UL)
//#define __RTC_XTAL  (32768UL)
//#define __IRC22M    (22118400UL)
//#define __IRC10K    (10000UL)
//#define __HSI       (__IRC22M)      /* Factory Default is internal 22MHz */

#define __USE_SYS_CLK__			__XTAL	

#define __USE_DELAY__
//#define __USE_WDT__
#define __USE_VALUE_CTRL__

#ifdef __USE_WDT__
#include "DrvTimer.h"
#endif

#define APROM_BASE_ADDR			0x00000000 //A21=0,A20=0
#define DATAFLASH_BASE_ADDR		0x0001F000 //A21=0,A20=0
#define LDROM_BASE_ADDR			0x00100000 //A21=0,A20=1
#define CONFIG_BASE_ADDR		0x00300000 //A21=1,A20=1
#define APROM_SIZE				0x00010000 //128K Bytes
//#define DATA_FLASH_SIZE			0x00001000 //4096 Bytes
#define LDROM_SIZE				0x00001000 //4096 Bytes

#define CONFIG0					0x00300000
#define CONFIG1					0x00300004

#define PAGE_SIZE				512

#define FMC_PAGE0				0
#define FMC_PAGE1				1*PAGE_SIZE
#define FMC_PAGE2				2*PAGE_SIZE
#define FMC_PAGE3				3*PAGE_SIZE
#define FMC_PAGE4				4*PAGE_SIZE
#define FMC_PAGE5				5*PAGE_SIZE
#define FMC_PAGE6				6*PAGE_SIZE
#define FMC_PAGE7				7*PAGE_SIZE

#define	VALUE_UP				1
#define	VALUE_DN				0

#ifdef __USE_DELAY__
void delay(unsigned long ulCNT);
void delay_us(unsigned long ulCNT);
void delay_ms(unsigned long ulCNT);
#endif

extern unsigned int DFB_Addr;

void FMC_Enable(void);
void FMC_Erase(unsigned int addr);
void FMC_Write_Byte(unsigned int addr, unsigned int w_data);
unsigned char FMC_Read_Byte(unsigned int addr);
void FMC_Write(unsigned int addr,unsigned int *w_data, unsigned int length);
void FMC_Read(unsigned int addr,unsigned int *r_data, unsigned int length);
void FMC_WriteConfig(unsigned int cfg_data0, unsigned int cfg_data1);

#ifdef __USE_WDT__
void wdt_enable(E_WDT_INTERVAL interval);
void wdt_reset(void);
#endif

unsigned int abs_diff(unsigned int A, unsigned int B);
unsigned char ascii_to_hex(unsigned char x, unsigned char y);
unsigned char ascii_to_dec(unsigned char x, unsigned char y);
void hex_to_ascii(unsigned char x, char *y);
void dec_to_ascii(unsigned char x, char *y, unsigned char len);

#ifdef __USE_VALUE_CTRL__
void value_updn(unsigned int change_value, unsigned char *value, unsigned int lo, unsigned int hi);
#endif
void FMC_Erase_test(unsigned int);
#endif
