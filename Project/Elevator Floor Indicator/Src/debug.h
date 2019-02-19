/*--------------------------------------------------------------*/
/* debug.h														*/
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

#ifndef __DEBUG_H__
#define __DEBUG_H__


/**********************************************************************************
										DEFINE
**********************************************************************************/
//#define MON_USE
#define MAX_PARA_NUM 10
#define COMMAND_BUF_SIZE 80
#define MON_PROMPT  "CMD>"

#define TIME_AFTER(unknown,known)	(((s32)(known)-(s32)(unknown))<0)
#define TIME_BEFORE(unknown,known)	(((s32)(unknown)-(s32)(known))<0)


//=============================================================================
//	extern variable
//=============================================================================
extern unsigned char ps_on;


//=============================================================================
//	function prototype
//=============================================================================
void mon_showHelp(void);
static void mon_parseCommand(unsigned char* com_buf, unsigned char len);
int uart_iskbhit(void);
unsigned char uart_getchar(void);
void mon_process(void);
#endif	//#ifndef __DEBUG_H__
