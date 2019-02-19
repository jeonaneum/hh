#ifndef __SOH701_H__
#define __SOH701_H__

#include "global.h"

// OSD Code Byte Define -----------------------------------------------
#define SOH7_ADDR					0xEA
#define SOH7_R_ADDR				0xEB
#define SOH7_STX						0xED	// start code
#define FOSD_ETX						0x00		// end code
#define FOSD_LOCATE_POS		0x01		// decide character position
#define FOSD_CHAR_WRITE		0x03		// write str
	#define PAGE_ENGLISH			1
	#define PAGE_KOREAN			2
	#define PAGE_JAPANESE			3
	#define PAGE_RUSSIAN			4
//	#define PAGE_MONGOLIA		5
//	#define PAGE_CHINESE			6
//	#define PAGE_CUSTOM			7
#define FOSD_CLEAR_ALL			0x05		// erase all characters
#define FOSD_CHAR_ONOFF		0x07		// character appear/disappear
#define FOSD_WORD_WRAP		0x0C
#define FOSDDAC_OUTPUT			0X17


void Osd_Init(void);
void Osd_Clear(void);
void Osd_Locate(unsigned char, unsigned char); 
void Osd_PutStr(unsigned char, unsigned char*);	
void Osd_PutChar(unsigned char, unsigned char*, unsigned char);
void Osd_Erase(unsigned char);

#endif


