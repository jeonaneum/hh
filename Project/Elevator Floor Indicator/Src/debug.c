/*--------------------------------------------------------------*/
/* debug.c														*/
/* routines for dvr system control.								*/
/*																*/
/* Copyright (c) 2011, SeeEyes Co.,Ltd.							*/
/* Harold Choi.													*/
/*																*/
/* Log:															*/
/* 																*/
/* Revision 0.1  11/11/24  14:51:00  Harold Choi				*/
/* First Release												*/
/*--------------------------------------------------------------*/	
#include <stdio.h>
#include <string.h>
#include <math.h>

#include "NUC1xx.h"
#include "common.h"
#include "uart.h"
#include "main.h"
#include "debug.h"

/**********************************************************************************
								EXTERNAL VARIABLES
**********************************************************************************/

/**********************************************************************************
										Variables
**********************************************************************************/

u08 loop=0;
s32 timeout=-1;
u08  cmdOld[COMMAND_BUF_SIZE];
u08  cmdLenOld=0;
static u08 CommandBuf[COMMAND_BUF_SIZE];
u08  CommandLength = 0;
u32 mon_repeatTick = 100;  // 1sec
volatile u32 tick_10ms = 0;
u08 ps_on=0;

/**********************************************************************************
										Functions
**********************************************************************************/
/*	
static u32 convHexStrToU32(u08 *str)
{
	u32 num=0, i;

	for (i=0;; i++)
	{
		if (str[i]==0)
			break;
		if (str[i]>='0' && str[i]<='9')
			num = (num<<4) + str[i] - '0';
		else if (str[i]>='A' && str[i]<='F')
			num = (num<<4) + str[i] - 'A' + 10;
		else if (str[i]>='a' && str[i]<='f')
			num = (num<<4) + str[i] - 'a' + 10;
	}
	return num;
}

static u08 convHexStrTou08(u08 *str)
{
	u08 num=0, i;

	for (i=0;; i++)
	{
		if (str[i]==0)
			break;
		if (str[i]>='0' && str[i]<='9')
			num = (num<<4) + str[i] - '0';
		else if (str[i]>='A' && str[i]<='F')
			num = (num<<4) + str[i] - 'A' + 10;
		else if (str[i]>='a' && str[i]<='f')
			num = (num<<4) + str[i] - 'a' + 10;
	}
	return num;
}
*/	
static u32 convDecStrToU32(u08 *c)
{
	u32 i,j;

	i=0;
	
	while(1) {
		if(*c==' ') { c++; continue; }
		if(*c==0 || *c < 0x30 || *c > 0x39) break;
		j=(*c)-0x30;
		i=i*10; 
		i+=j;
		c++;		
	}

	return i;
}

void mon_showHelp(void)
{
	printf("\r\n");
	printf("==========================================\r\n");
	printf("<command list>\r\n");
	printf("help : help command\r\n");
	printf("reg : read register\r\n");
	printf("3 :\r\n");	
	printf("==========================================\r\n");

}

static void mon_parseCommand(u08* com_buf, u08 len)
{

	u08 i;
	u08 space_flag=TRUE;
	u08 argc=0;
	u08 *argv[MAX_PARA_NUM];
	
	for(i=0;i<len;i++){
        if(!(com_buf[i] >= '0' && com_buf[i] <= '~')){
			space_flag=TRUE;
			com_buf[i]=0;
		}
        else{
			if(space_flag==TRUE){
				space_flag=FALSE;
				argv[argc++]=&com_buf[i];
				if(argc >= MAX_PARA_NUM)
                    break;
			}
		}
	}
    
    if(len < MAX_PARA_NUM)  com_buf[len] = 0;
    
    if(argc == 0)   return;

	if(!strcmp((char*)argv[0],"help")){
		mon_showHelp();
	}
	else if(!strcmp((char*)argv[0],"ps_on")){
		ps_on = ON;
		printf("manual pressure set on\r\n");
	}
	
	else if(!strcmp((char*)argv[0],"ps_off")){
		ps_on = OFF;	
		printf("manual pressure set off\r\n");
	}
	
	else if(!strcmp((char*)argv[0],"ps")){

		if(ps_on){
//			presInfo.avgPres = pres;
//			printf("presInfo.avgPres=%d\r\n",presInfo.avgPres);
		}
		else{
			printf("ps_on=%d check please!!!!!!!!!\r\n",ps_on);
		}
	}
	
	else if(!strcmp((char*)argv[0],"sf")){		//현재층 정보를 설정한 값으로 Flash에 저장(1~99층 설정가능), ex) sf 01 => 1 floor, sf 99 => 99 floor
		u08 temp;

		temp = (u08)(convDecStrToU32(argv[1]));
		if(temp > 0 && temp < 100){
//			FloorInfo.curFloor = (u08)(convDecStrToU32(argv[1])) + 10;
//			CFG_Save(PAGE1);
//			presInfo.initPres = FALSE;
//			CFG_Init();
		}
		else
			printf("Input value Error!!(range:1~99)\r\n");
	}
	else if(!strcmp((char*)argv[0],"tick")){		//Fosc=12MHz, so 12000000/12000=1000Hz=1ms
		u32 msCount=0;
		u32 regSYCT_RVR=0;

		msCount = 1;

		regSYCT_RVR = 12000L*msCount;		
		printf("mscount=0x%04x,regSYCT_RVR=0x%04x\r\n",msCount,regSYCT_RVR);

		regSYCT_RVR = 12000L*msCount;		
		printf("mscount=0x%04x,regSYCT_RVR=0x%04x\r\n",msCount,regSYCT_RVR);
		
		regSYCT_RVR = 120000L*msCount;		
		printf("mscount=0x%04x,regSYCT_RVR=0x%04x\r\n",msCount,regSYCT_RVR);

		regSYCT_RVR = 1200000L*msCount;		
		printf("mscount=0x%04x,regSYCT_RVR=0x%04x\r\n",msCount,regSYCT_RVR);

		regSYCT_RVR = 12000000L*msCount;		
		printf("mscount=0x%04x,regSYCT_RVR=0x%04x\r\n",msCount,regSYCT_RVR);
	}
}

int uart_iskbhit(void)
{
	if(UART1->FSR.RX_EMPTY == 0x00)
        return 1;
	else
		return 0;
}

u08 uart_getchar(void)
{
     unsigned char ch;

	if(UART1->FSR.RX_EMPTY != 0x00)
		return 0;
	ch = UART1->DATA;
	return ch;
}

void mon_process(void)
{
	u08 ch;
    
    if(loop){
        if(TIME_AFTER(tick_10ms,timeout)){
            mon_parseCommand(cmdOld,cmdLenOld);
            timeout = tick_10ms+mon_repeatTick;
        }
    }
	if(uart_iskbhit()){
		ch = uart_getchar();
        if(ch == 0x0a) return;

        if(ch == 0x03){
            loop = 0;
			printf("%s",MON_PROMPT);
            return;
        }
		CommandBuf[CommandLength] = ch;

		if(ch == '\b'){
			if(CommandLength!=0){
                CommandBuf[CommandLength] = 0;
                CommandLength--;
            }
		}
        else{
			if(CommandLength < (COMMAND_BUF_SIZE-1))
                CommandLength++;
		}

		if(ch==0x0d){
			printf("\r\n");
			CommandBuf[CommandLength] = 0;
            CommandLength--;
            if(CommandLength < COMMAND_BUF_SIZE) CommandBuf[CommandLength] = 0;
			if(CommandLength > 0){
                if(strcmp((char*)CommandBuf,"repeat") == 0){
                    loop = 1;
                }
                else{
                    mon_parseCommand(CommandBuf,CommandLength);
                    memcpy(cmdOld,CommandBuf,CommandLength);
                    memset(CommandBuf,0,sizeof(CommandBuf));
                    cmdLenOld = CommandLength;
                }
            }
			CommandLength = 0;
			printf("\r\n");
			printf("%s",MON_PROMPT);
		}
		else
			printf("%c",(u08)ch);
	}
}

