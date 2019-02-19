#include <stdio.h>
#include <string.h>
#include "main.h"
#include "global.h"
#include "FMC.h"

#include "common.h"
#include "uart.h"
#include "NUC1xx.h"
#include "cfg_def.h"

/////////////////////////////////////////////////////////////////////////////

c08 default_page0[]={
	CFG_CHK_CODE1,			// ADDR_CHK_CODE1	0	->	save data eeprom addr
	CFG_CHK_CODE2,			// ADDR_CHK_CODE2	1	->	save data eeprom addr
	CFG_CHK_CODE3,			// ADDR_CHK_CODE3	2	->	save data eeprom addr
	CFG_CHK_CODE4,			// ADDR_CHK_CODE4	3	->	save data eeprom addr
	ADDR_MAX_SET,			// sys config change check	9
};

c32 default_page1[]={
	DEFAULT_BASE_FLOOR,		// BASE FLOOR
	DEFAULT_BASE_FLOOR,		// CURRENT FLOOR
	DEFAULT_OSD_POS,		// OSD POSITION
	DEFAULT_RS485_ID,		// rs485_id[0]
	DEFAULT_RS485_ID,		// rs485_id[1]
	DEFAULT_RS485_ID,		// rs485_id[2]
	DEFAULT_MAX_FLOOR,
	DEFAULT_MIN_FLOOR,
	DEFAULT_DOOR,			// door_max
	DEFAULT_DOOR,			// door_min
//	DEFAULT_VIDEO,
};
u08 i;
// FMC Control -----------------------------------------------
void CFG_Init(void)
{
	u32 flash_buff[FMC_SIZE];
	u32 buff[128];
#ifdef __USE_WDT__	
	wdt_reset();
#endif
		
	DFB_Addr = CONFIG0;
	FMC_Read(0,&flash_buff[0],4);
	DFB_Addr = FMC->DFBADR;
#ifdef _DEBUG_
	printf("CFG_Init[CFG0 = 0x%08x, DFBADR = 0x%08x]\n",flash_buff[0], FMC->DFBADR);
#endif
	
	if((flash_buff[0] != CFG0_SET_VAL) || (DFB_Addr != CFG1_SET_VAL))
	{
		FMC_WriteConfig(CFG0_SET_VAL, CFG1_SET_VAL);
		DFB_Addr = CONFIG0;
		FMC_Read(0,&flash_buff[0],4);
		DFB_Addr = FMC->DFBADR;

		#ifdef _DEBUG_
		printf("Config error. Reset(Config0=0x%08x, Config1=0x%08x)\n",flash_buff[0],DFB_Addr);
		#endif
	}
	//------------------------------------------------------------------------------------------
	
	FMC_Read(PAGE0,&flash_buff[0], 8);
	
	if(flash_buff[0] != CFG_CHK_CODE){
		#ifdef _DEBUG_
		printf("\nCFG_CHK_CODE error(R=0x%08x,C=0x%08x). CFG_Default...\n",flash_buff[0],CFG_CHK_CODE);
		#endif

		CFG_Dflt();
	}

	if(flash_buff[1] != ADDR_MAX_SET){
		#ifdef _DEBUG_
		printf("\nConfig size changed(%d->%d). CFG_Default...\n",flash_buff[1],ADDR_MAX_SET);
		#endif

		CFG_Dflt();
	}
	
#if 0
	{	///flash memory data save with mini USB
		memcpy(&buff, &default_page1, 40);	// if video add, 44
		FMC_Write(FMC_PAGE1,(u32*)buff,FMC_PAGE1_SIZE);
		
		FMC_Read(FMC_PAGE1,(u32*)buff,FMC_PAGE1_SIZE);
		
		for(i=0; i<110; i++)
		{
			Default_FloorInfo_to_OSDInfo(i);
			buff[i] = osd_floor[i][0] << 8;
			buff[i] |= osd_floor[i][1];
		}
		FMC_Write(FMC_PAGE2,(u32*)buff,FMC_PAGE2_SIZE);
	}
#endif
	
	CFG_Read(PAGE0);
	CFG_Read(PAGE1);
	CFG_Read(PAGE2);
//	FMC_Read(FMC_PAGE2,(u32*)flash_buff,FMC_PAGE2_SIZE);
//	for(i=0; i<FMC_PAGE2_SIZE; i++)
//		printf("flash_buff[%d] : %x \n", i, flash_buff[i]);
//	FMC_Read(FMC_PAGE1,(u32*)flash_buff,FMC_PAGE1_SIZE);
//		for(i=0; i<11; i++)
//		printf("flash_buff[%d] : %x \n", i, flash_buff[i]);
//	printf("--------------------\n");
	
//	printf("Floor.current : %d \n", Floor.current);
//	printf("Floor.base : %d \n", Floor.base);
//	printf("video_signal : %d \n", video_signal);
	
}

void CFG_Dflt(void)
{
	u08 flash_buff[PAGE_SIZE];
	u32 buff[128];
	u08 i;
	
	memset(&flash_buff,0,PAGE_SIZE);

	FMC_Erase(FMC_PAGE0);
	FMC_Erase(FMC_PAGE1);
	FMC_Erase(FMC_PAGE2);
	FMC_Erase(FMC_PAGE3);
	FMC_Erase(FMC_PAGE4);
	FMC_Erase(FMC_PAGE5);

	memcpy(&flash_buff, &default_page0, FMC_PAGE0_SIZE);
	FMC_Write(FMC_PAGE0,(u32*)flash_buff,FMC_PAGE0_SIZE);
		
	memcpy(&buff, &default_page1, 40);	// if video add, 44
	FMC_Write(FMC_PAGE1,(u32*)buff,FMC_PAGE1_SIZE);
	
	FMC_Read(FMC_PAGE1,(u32*)buff,FMC_PAGE1_SIZE);
//		for(i=0; i<11; i++)
//		printf("buff[%d] : %x \n", i, buff[i]);
	
	for(i=0; i<110; i++)
	{
		Default_FloorInfo_to_OSDInfo(i);
		buff[i] = osd_floor[i][0] << 8;
		buff[i] |= osd_floor[i][1];
	}
	FMC_Write(FMC_PAGE2,(u32*)buff,FMC_PAGE2_SIZE);
	FMC_Read(FMC_PAGE2,(u32*)buff,FMC_PAGE2_SIZE);
//	for(i=0; i<FMC_PAGE2_SIZE; i++)
//		printf("buff[%d] : %x \n", i, buff[i]);
}

void Default_FloorInfo_to_OSDInfo(u08 temp)
{
	if(!temp)
	{
		osd_floor[temp][0] = '0';
		osd_floor[temp][1] = '1';
	}
	else if(temp < 10)
	{
		osd_floor[temp][0] = 'B';
		osd_floor[temp][1] = '0' + (10 - temp);	
	}
	else if(temp == 10)
	{
		osd_floor[temp][0] = 'X';
		osd_floor[temp][1] = ' ';
	}
	else	
	{
		osd_floor[temp][0] = '0' + ((temp / 10) - 1);
		osd_floor[temp][1] = '0' + (temp % 10);
	}
}

void CFG_Read(u32 page)
{
	u32 buff[FMC_SIZE];
	u08 i;
	
#ifdef __USE_WDT__	
	wdt_reset();
#endif

	switch(page)
	{
		case PAGE0:	
			FMC_Read(FMC_PAGE0,(u32*)buff,FMC_PAGE0_SIZE);
		break;
		case PAGE1: 
			FMC_Read(FMC_PAGE1,(u32*)buff,FMC_PAGE1_SIZE);
//			for(i=0; i<8; i++)
//				printf("buff[%d] : %x \n", i, buff[i]);
			Floor.current = (u08)buff[ADDR_CUR_FLOOR];
			Floor.base = (u08)buff[ADDR_BASE_FLOOR];
			osd_position = (u08)buff[ADDR_OSD_POS];
			rs485_id[0] = (u08)buff[ADDR_ID_0];
			rs485_id[1] = (u08)buff[ADDR_ID_1];
			rs485_id[2] = (u08)buff[ADDR_ID_2];
			Floor.max = (u08)buff[ADDR_MAX_FLOOR];
			Floor.min = (u08)buff[ADDR_MIN_FLOOR];
			door_max = buff[ADDR_MAX_DOOR]; 
			door_min = buff[ADDR_MIN_DOOR];
//			video_signal = (u08)buff[ADDR_VIDEO];	
		break;
		case PAGE2:
			FMC_Read(FMC_PAGE2, buff, FMC_PAGE2_SIZE);
//			for(i=0; i<FMC_PAGE2_SIZE; i++)
//			printf("buff[%d] : %x \n", i, buff[i]);
			for(i=0; i<110; i++)
			{
				osd_floor[i][0] = (buff[i] >> 8);
				osd_floor[i][1] = (buff[i] & 0xff);
			}
		break;
			
#ifdef DEBUG_MSG_OUT						
						//printf("CFG_Read=>Floor_Osd[%d][0]=%d Floor_Osd[%d][1]=%d\r\n",i, Floor_Osd[i][0], i, Floor_Osd[i][1]);
#endif						

		default:	break;
	}
}

void CFG_Save(u32 page)
{
	u32 buff[FMC_SIZE];
	u08 i;
#ifdef __USE_WDT__	
	wdt_reset();
#endif
	
	switch(page)
	{
		case PAGE0:
			memcpy(&buff[ADDR_CHK_CODE1], &default_page0, 5);
			FMC_Write(FMC_PAGE0,(u32*)buff,FMC_PAGE0_SIZE);
		break;
		case PAGE1:
			buff[ADDR_BASE_FLOOR] = (u32)Floor.base;						//reset floor info
			buff[ADDR_CUR_FLOOR] = (u32)Floor.current;						//current floor info
			buff[ADDR_OSD_POS] = (u32)osd_position; 						//osd position info
			buff[ADDR_ID_0] = (u32)rs485_id[0];
			buff[ADDR_ID_1] = (u32)rs485_id[1];
			buff[ADDR_ID_2] = (u32)rs485_id[2];
			buff[ADDR_MAX_FLOOR] = (u32)Floor.max;
			buff[ADDR_MIN_FLOOR] = (u32)Floor.min;
			buff[ADDR_MAX_DOOR] = (u32)door_max;
			buff[ADDR_MIN_DOOR] = (u32)door_min;	
//			buff[ADDR_VIDEO] = (u32)video_signal;
			FMC_Write(FMC_PAGE1, (u32*)buff, FMC_PAGE1_SIZE);
		break;
		case PAGE2:
			for(i=0; i<110; i++)
			{
				buff[i] = osd_floor[i][0] << 8;
				buff[i] |= osd_floor[i][1];
			}
			FMC_Write(FMC_PAGE2, (u32*)buff, FMC_PAGE2_SIZE);
		break;
			
#ifdef DEBUG_MSG_OUT
						//printf("CFG_Save=>osd_char[0]=%d osd_char[1]=%d\r\n",i, osd_char[0], i, osd_char[1]);
#endif						
		default:	break;
	}
}
	
