/*---------------------------------------------------------------------------------------------------------*/
/*                                                                                                         */
/* Copyright(c) 2010 Nuvoton Technology Corp. All rights reserved.                                         */
/*                                                                                                         */
/*---------------------------------------------------------------------------------------------------------*/

#ifndef __UART_H__
#define __UART_H__

#include "DrvUART.h"

#define _DEBUG_

#define _USE_UART0_
#define _USE_UART1_

#define UART_0		0
#define UART_1		1
#define DEBUG_PORT	UART_0		// Using debug 0:UART0  1:UART1 2:UART2
#define CTRL_PORT	UART_0		// ex-ctrl rs232/485 0:UART0  1:UART1 2:UART2

#define RS485_PORT	UART_1	// Using rs485 0:UART0  1:UART1 2:UART2

//==========================================================================
#define UARTBUFSIZE		0x80
#define UARTBUFMASK		UARTBUFSIZE-1

enum{
	BAUD1200 = 0,
	BAUD2400,
	BAUD4800,
	BAUD9600,
	BAUD19_2K,
	BAUD38_4K,
	BAUD57_6K,
	BAUD115_2K
};

typedef struct UART_Ctrl_Tx{
	unsigned char buff[UARTBUFSIZE];
//	unsigned char data[UARTBUFSIZE];
	unsigned char buff_cnt;
	unsigned char data_cnt;
//	unsigned char data_len;
	unsigned char busy_flag;
} UART_Ctrl_Tx;

typedef struct UART_Ctrl_Rx{
	unsigned char buff[UARTBUFSIZE];
	unsigned char data[UARTBUFSIZE];
	unsigned char buff_cnt;
//	unsigned char read_cnt;
//	unsigned char wait_cnt;
	unsigned char recv_flag;
	unsigned char step;
} UART_Ctrl_Rx;


//==========================================================================
#ifdef _USE_UART0_
extern UART_Ctrl_Tx tx0;
extern UART_Ctrl_Rx rx0;
#endif
#ifdef _USE_UART1_
extern UART_Ctrl_Tx tx1;
extern UART_Ctrl_Rx rx1;
#endif

//==========================================================================
void UART_Init(unsigned char ch, unsigned char baud);
//void ModeSet(u08 ch);
//void BaudSet(u08 ch, u08 baud);
void Dbg_RxInt_Proc(UART_Ctrl_Rx *rx);
void UartTxInt_Proc(E_UART_PORT u32Port, UART_Ctrl_Tx *txd);
void uart_send(E_UART_PORT u32Port, unsigned char *txd, unsigned char length);
void uart_gets(unsigned char *rxd, unsigned char *rxb, unsigned char length);

unsigned int UART_Open(E_UART_PORT u32Port, STR_UART_T *sParam);
void SetRTS(E_UART_PORT u32Port,uint8_t u8Value,uint16_t u16TriggerLevel);
void SetFnRS485(E_UART_PORT u32Port,STR_RS485_T *str_RS485);

#endif
