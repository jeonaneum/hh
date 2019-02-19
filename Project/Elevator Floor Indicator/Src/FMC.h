#ifndef __FMC_h__
#define __FMC_h__


// FMC Code Byte Define -----------------------------------------------

//PAGE 0
enum {
	ADDR_CHK_CODE1=0,
	ADDR_CHK_CODE2,
	ADDR_CHK_CODE3,
	ADDR_CHK_CODE4,
	ADDR_MAX_SET = ADDR_CHK_CODE4,
};
#define FMC_PAGE0_SIZE				5

//PAGE 1
enum{
	ADDR_BASE_FLOOR=0,
	ADDR_CUR_FLOOR,
	ADDR_OSD_POS,
	ADDR_ID_0,
	ADDR_ID_1,
	ADDR_ID_2,
	ADDR_MAX_FLOOR,
	ADDR_MIN_FLOOR,
	ADDR_MAX_DOOR,
	ADDR_MIN_DOOR,
	ADDR_VIDEO,
};
#define FMC_PAGE1_SIZE				11

#define DEFAULT_BASE_FLOOR			11
#define DEFAULT_OSD_POS				0
#define DEFAULT_RS485_ID			0
#define DEFAULT_MAX_FLOOR			109
#define DEFAULT_MIN_FLOOR			1
#define DEFAULT_DOOR				0
#define DEFAULT_VIDEO				0

//PAGE 2
#define FMC_PAGE2_SIZE				110
#define DEFAULT_OSD_CHAR			0


#define FMC_SIZE					110

#define PAGE0										0
#define PAGE1										1
#define PAGE2										2
#define PAGE3										3
#define PAGE4										4
#define PAGE5										5
#define PAGE6										6
#define PAGE7										7
//-----------------------------------------------------

//-----------------------------------------------------
void CFG_Save(unsigned int);
void CFG_Read(unsigned int);
void CFG_Init(void);
void CFG_Dflt(void);

#endif
