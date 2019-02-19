/*---------------------------------------------------------------------------------------------------------*/
/*                                                                                                         */
/* Copyright(c) 2010 Nuvoton Technology Corp. All rights reserved.                                         */
/*                                                                                                         */
/*---------------------------------------------------------------------------------------------------------*/

#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "NUC1xx.h"
#include "DrvSYS.h"
#include "DrvGPIO.h"
//#include "DrvUSB.h"
#include "terminaldriver.h"
#include "common.h"
#include "global.h"
#include "uart.h"
#include "main.h"

//#include "USB\VCOM_API.h"

//#include "protocol.h"

#ifdef _DEBUG_
char _get_char(void);
/*
#if defined ( __CC_ARM   )
#if (__ARMCC_VERSION < 400000)
#else
// Insist on keeping widthprec, to avoid X propagation by benign code in C-lib
#pragma import _printf_widthprec
#endif
#endif
*/
/*---------------------------------------------------------------------------------------------------------*/
/* Global variables                                                                                        */
/*---------------------------------------------------------------------------------------------------------*/
struct __FILE { int32_t handle; /* Add whatever you need here */ };
FILE __stdout;
FILE __stdin;

/*-----------------------------------------------------------------------------
  Function:		_put_char                                                                         
  Parameter:    ch:	char is going to write.                                     
  Returns:      None                                                                                      
  Description:  Routine to write a char.                                    
 *-----------------------------------------------------------------------------*/
void _put_char(int32_t ch)
{
#if(DEBUG_PORT == UART_1)
	if(ch == '\n'){
		while(UART1->FSR.TX_EMPTY == 0);
		UART1->DATA = '\r';
	}
	while(UART1->FSR.TX_EMPTY == 0);
	UART1->DATA = ch;
#elif(DEBUG_PORT == UART_0)
	if(ch == '\n'){
		while(UART0->FSR.TX_EMPTY == 0);
		UART0->DATA = '\r';
	}
	while(UART0->FSR.TX_EMPTY == 0);
	UART0->DATA = ch;
#endif
/*
	if(ch == '\n'){
		gTxBuf[0] = '\r';
		gTxBuf[1] = '\n';
		gu32TxSize = 2;
		DrvUSB_DataIn(BULK_IN_EP_NUM, gTxBuf, gu32TxSize);
		while(gu32TxSize);
	}
	else{
		gTxBuf[0] = ch;
		gu32TxSize = 1;
		DrvUSB_DataIn(BULK_IN_EP_NUM, gTxBuf, gu32TxSize);
		while(gu32TxSize);
	}
*/
}

/*-----------------------------------------------------------------------------
  Function:		_get_char                                                                         
  Parameter:    None                                     
  Returns:      A char                                                                                      
  Description:  Routine to get a char.                                    
 *-----------------------------------------------------------------------------*/
char _get_char(void)
{
	while(1){
#if(DEBUG_PORT == UART_1)
		if(UART1->FSR.RX_EMPTY == 0x00)	return ((uint8_t)UART1->DATA);
#elif(DEBUG_PORT == UART_0)
		if(UART0->FSR.RX_EMPTY == 0x00)	return ((uint8_t)UART0->DATA);
#endif
	}
}

/*---------------------------------------------------------------------------------------------------------*/
/* C library retargetting                                                                                  */
/*---------------------------------------------------------------------------------------------------------*/
void _ttywrch(int32_t ch){ _put_char(ch); return; }
int fputc(int32_t ch, FILE *f){ _put_char(ch); return 0; }
int fgetc(FILE *f){ return (_get_char()); }
int ferror(FILE *f){ return EOF; }
#endif

extern void UartRxInt_Proc(UART_Ctrl_Rx *rx);

//////////////////////////////////////////////////////////////////////////////
#ifdef _USE_UART0_
UART_Ctrl_Tx tx0;
UART_Ctrl_Rx rx0;
#endif
#ifdef _USE_UART1_
UART_Ctrl_Tx tx1;
UART_Ctrl_Rx rx1;
#endif

//=============================================================================
#ifdef _USE_UART0_
void UART02_IRQHandler(void)
{
	if(UART0->ISR.RDA_INT)
	{
		if(UART0->FSR.RX_EMPTY == 0x00)	rx0.buff[rx0.buff_cnt] = UART0->DATA;
		//UART0->DATA = rx0.buff[rx0.buff_cnt];

#if(DEBUG_PORT == UART_0)
	#ifdef _DEBUG_
		Dbg_RxInt_Proc(&rx0);
	#else
		UartRxInt_Proc(&rx0);
	#endif
#else
		UartRxInt_Proc(&rx0);
#endif
	}
	else if(UART0->ISR.THRE_INT)
	{   
		UartTxInt_Proc(UART_PORT0,&tx0);
	}
//	else if(UART0->ISR.TOUT_INT){				// Rx Timeout Int.
//		rx0.buff_cnt = 0; rx0.step = 0;	UART0->IER.RTO_IEN = 0;
//	}
}
#endif
#ifdef _USE_UART1_
void UART1_IRQHandler(void)
{
	if(UART1->ISR.RDA_INT)
	{
		if(UART1->FSR.RX_EMPTY == 0x00)	rx1.buff[rx1.buff_cnt] = UART1->DATA;

#if(DEBUG_PORT == UART_1)
	#ifdef _DEBUG_
		Dbg_RxInt_Proc(&rx1);
	#else
		UartRxInt_Proc(&rx1);
	#endif
#else

		UartRxInt_Proc(&rx1);
#endif
	}
	else if(UART1->ISR.THRE_INT)
	{   
		UartTxInt_Proc(UART_PORT1,&tx1);
	}
//	else if(UART1->ISR.TOUT_INT){				// Rx Timeout Int.
//		rx1.buff_cnt = 0; rx1.step = 0;	UART1->IER.RTO_IEN = 0;
//	}
}
#endif

/*-----------------------------------------------------------------------------
  Function:		UART_Init   
  Parameter:    None                                     
  Returns:      None                                                                                      
  Description:  Initialize UART
 *-----------------------------------------------------------------------------*/
void UART_Init(u08 ch, u08 baud)
{
	STR_UART_T sParam;
//	STR_RS485_T sParam_RS485;

#ifdef _USE_UART0_
	if(ch == 0){
		DrvGPIO_InitFunction(E_FUNC_UART0);
		// Set UART Configuration
	    sParam.u32BaudRate 		= baud;
	    sParam.u8cDataBits 		= DRVUART_DATABITS_8;
	    sParam.u8cStopBits 		= DRVUART_STOPBITS_1;
	    sParam.u8cParity 		= DRVUART_PARITY_NONE;
	    sParam.u8cRxTriggerLevel= DRVUART_FIFO_1BYTES;
		sParam.u8TimeOut        = 0x7F;
	 	UART_Open(UART_PORT0,&sParam);
/*
		// Set RS485 Configuration
		sParam_RS485.u8cAddrEnable = DISABLE; 
		sParam_RS485.u8cAddrValue  = 0;
		sParam_RS485.u8cModeSelect = MODE_RS485_NMM|MODE_RS485_AUD;
		sParam_RS485.u8cDelayTime  = 0;
		sParam_RS485.u8cRxDisable  = TRUE;
		SetFnRS485(UART_PORT0,&sParam_RS485);
		SetRTS(UART_PORT0,1,0);
*/
//		SYS->GPBMFP.UART0_nRTS_nWRL = 1;
//		UART0->FUNSEL.FUN_SEL 	= FUN_RS485;
//		UART0->ALTCON.RS485_NMM	= 1;
//		UART0->ALTCON.RS485_AUD	= 1;

		UART0->IER.RDA_IEN = 1;			// 
		NVIC_EnableIRQ(UART0_IRQn);		// Enable UART0 Interrupt

		memset(&tx0,0,sizeof(UART_Ctrl_Tx));
		memset(&rx0,0,sizeof(UART_Ctrl_Rx));
	}
#endif
#ifdef _USE_UART1_
	if(ch == 1){
		DrvGPIO_InitFunction(E_FUNC_UART1);
		// Set UART Configuration
	    sParam.u32BaudRate 		= baud;
	    sParam.u8cDataBits 		= DRVUART_DATABITS_8;
	    sParam.u8cStopBits 		= DRVUART_STOPBITS_1;
	    sParam.u8cParity 		= DRVUART_PARITY_NONE;
	    sParam.u8cRxTriggerLevel= DRVUART_FIFO_1BYTES;
		sParam.u8TimeOut        = 0x7F;
	 	UART_Open(UART_PORT1,&sParam);

		UART1->IER.RDA_IEN = 1;			// 
		NVIC_EnableIRQ(UART1_IRQn);		// Enable UART1 Interrupt
	
		memset(&tx1,0,sizeof(UART_Ctrl_Tx));
		memset(&rx1,0,sizeof(UART_Ctrl_Rx));
		
	}
#endif
}

//--------------------------------------------------------------------------------------------------
#ifdef _DEBUG_
void Dbg_RxInt_Proc(UART_Ctrl_Rx *rx)
{
	if(rx->buff[rx->buff_cnt] == 0x0d)	rx->buff[rx->buff_cnt] = '>';	// 0x0d : enter
#if(DEBUG_PORT == UART_0)
	UART0->DATA = rx->buff[rx->buff_cnt];
#elif(DEBUG_PORT == UART_1)
	UART1->DATA = rx->buff[rx->buff_cnt];
#endif

	if(rx->buff[rx->buff_cnt] == '>'){									// debug command receive
		memset(&rx->data, 0, sizeof(rx->data));
		uart_gets(rx->data, rx->buff, rx->buff_cnt);
		rx->buff_cnt = 0;
		rx->recv_flag = 3;
	}
	else if(rx->buff[rx->buff_cnt] == 0x08){							// 0x08 : back space
		if(rx->buff_cnt)	rx->buff_cnt--;								// Erases from the current cursor position to the end of the current line.
	//	_put_char(0x7f);												// \e(esc)
		_put_char(0x1b);												// \e(esc)
		_put_char(0x5b);												// '['
		_put_char(0x4b);												// 'K'
	}
	else if(rx->buff[rx->buff_cnt] >= 0x20){							// 0x20 : space bar ' '
		rx->buff_cnt++;
		rx->buff_cnt &= UARTBUFMASK;
	}
}
#endif

void UartTxInt_Proc(E_UART_PORT u32Port, UART_Ctrl_Tx *txd)
{
	UART_T * tUART;
	tUART = (UART_T *)((unsigned int)UART0 + u32Port);  

	if(txd->buff_cnt != txd->data_cnt){
		tUART->DATA = txd->buff[txd->buff_cnt];
		txd->buff_cnt++;
		txd->buff_cnt &= UARTBUFMASK;
	}
	else{
		txd->data_cnt = 0;
		txd->buff_cnt = 0;
		txd->busy_flag = 0;
		tUART->IER.THRE_IEN = 0;
#ifdef RS485_EN_LO
		while(tUART->FSR.TE_FLAG == 0);
		RS485_EN_LO;
#endif
	}
}

void uart_send(E_UART_PORT u32Port, u08 *txd, u08 length)
{
	u08 i;
	UART_T * tUART;
	UART_Ctrl_Tx *tx = 0;

	tUART = (UART_T *)((unsigned int)UART0 + u32Port);  
#ifdef _USE_UART0_
	if(u32Port == UART_PORT0)	tx = &tx0;
#endif
#ifdef _USE_UART1_
	if(u32Port == UART_PORT1)	tx = &tx1;
#endif

	for(i=0;i<length;i++){
//		printf("%02x ", tx->buff[tx->data_cnt]);
		tx->buff[tx->data_cnt++] = txd[i];
		tx->data_cnt &= UARTBUFMASK;
	}
//	printf("\n");
	if(tx->busy_flag == 0){
#ifdef RS485_EN_HI
		RS485_EN_HI;
//		printf("RS485_EN_HI!! -> %d", GPB_14);
#endif
		tx->busy_flag = 1;
		tUART->DATA = tx->buff[tx->buff_cnt++];
		tUART->IER.THRE_IEN = 1;
		tx->buff_cnt &= UARTBUFMASK;
	}
}

void uart_gets(u08 *rxd, u08 *rxb, u08 length)
{
	u08 i;
	
	for(i=0; i<=length; i++) rxd[i] = rxb[i];
}


/*---------------------------------------------------------------------------------------------------------*/
/* Function:     BaudRateCalculator                                                                        */
/*                                                                                                         */
/* Parameter:        																					   */	
/*	             clk          		-[in] Uart Source Clock                                                */
/*	             baudRate     		-[in] User seting BaudRate                                             */
/*	             UART_BAUD_T *baud  -[in] Get User Settings                                                */
/* Returns:                                                                                                */
/*               None                                                                                      */
/* Description:                                                                                            */
/*               The function is used to set Baud Rate register according user's settings                  */
/*---------------------------------------------------------------------------------------------------------*/

/*-----BaudRate Configure----------------------------------------------------------------------------------*/

/*

Mode	DIV_X_EN	DIV_X_ONE	Divider X	BRD	(Baud rate equation)
-----------------------------------------------------------------------
0	    Disable	0	B	        A	        UART_CLK / [16 * (A+2)]
1	    Enable	0	B	        A	        UART_CLK / [(B+1) * (A+2)] , B must >= 8
2	    Enable	1	Don't care	A	        UART_CLK / (A+2), A must >=3

*/
static void BaudRateCalculator(unsigned int clk, unsigned int baudRate, E_UART_PORT u32Port)
{
  	int tmp;
	int div;
 	unsigned int baud_table[8] = {1200, 2400, 4800, 9600, 19200, 38400, 57600, 115200};
 
  	UART_T * tUART;
	tUART = (UART_T *)((unsigned int)UART0 + u32Port);  
	baudRate = baud_table[baudRate];
	
	if(((clk / baudRate)%16)<3){			// Source Clock mod 16 <3 => Using Divider X =16 (MODE#0)
		tUART->BAUD.DIV_X_EN = 0;
	    tUART->BAUD.DIV_X_ONE   = 0;
		tmp = clk / baudRate/16  -2;
	}
	else{									// Source Clock mod 16 >3 => Up 5% Error BaudRate
	    tUART->BAUD.DIV_X_EN = 1;			// Try to Set Divider X = 1 (MODE#2)
	    tUART->BAUD.DIV_X_ONE   = 1;
		tmp = clk / baudRate  -2;

		if(tmp > 0xFFFF){					// If Divider > Range
			tUART->BAUD.DIV_X_EN = 1;		// Try to Set Divider X up 10 (MODE#1)
			tUART->BAUD.DIV_X_ONE   = 0;

			for(div = 8; div <16;div++){
				if(((clk / baudRate)%(div+1))<3){
					tUART->BAUD.DIVIDER_X   = div;
					tmp = clk / baudRate / (div+1) -2;
					break;
				}
			}
		}
	}

	tUART->BAUD.BRD = tmp; 
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:     GetUartClk                                                                       		   */
/*                                                                                                         */
/* Parameter:        																					   */	
/*               None                                                                                      */
/* Returns:                                                                                                */
/*               Current Uart Clock  (Hz)                                                                  */
/* Description:                                                                                            */
/*               The function is used to get Uart clock                                                    */
/*---------------------------------------------------------------------------------------------------------*/
static unsigned int GetUartClk(void)
{
	unsigned int clk =0 , div;

    div = SYSCLK->CLKDIV.UART_N;				// According PLL Clock and UART_Divider to get clock
													
	if(SYSCLK->CLKSEL1.UART_S == 0)			    // Check UART Clock Source Setting
		clk = DrvSYS_GetExtClockFreq();			// Get External Clock From DrvSYS Setting
	else if(SYSCLK->CLKSEL1.UART_S == 1)
		clk = DrvSYS_GetPLLClockFreq();
	else
		clk = __IRC22M;							// Clock 22Mhz

	return (clk/(div+1));
}

/*---------------------------------------------------------------------------------------------------------*/
/* Function:     DrvUART_Open                                                                        	   */
/*                                                                                                         */
/* Parameter:        																					   */	
/*				 u32Port -[in] UART Channel:  UART_PORT0 / UART_PORT1 /UART_PORT2                  		   */
/*               sParam  -[in] the struct parameter to configure UART                                      */
/*                         include of                                                                      */
/*                             u32BaudRate    - Baud rate 												   */									        
/*                                                                                                         */
/*                             u8cParity - DRVUART_PARITY_NONE / DRVUART_PARITY_EVEN / DRVUART_PARITY_ODD  */
/*                                                                                                         */
/*                             u8cDataBits - DRVUART_DATA_BITS_5 / DRVUART_DATA_BITS_6 		               */
/*									  	 	      DRVUART_DATA_BITS_7 / DRVUART_DATA_BITS_8        		   */
/*                                                                                                         */
/*                             u8cStopBits - DRVUART_STOPBITS_1 / STOPBITS_1_5 / STOPBITS_2   		       */
/*                                                                                                         */
/*                             u8cRxTriggerLevel   - DRVUART_FIFO_1BYTES to DRVUART_FIFO_62BYTES           */
/*                                                                                                         */
/*                             u8TimeOut - Time out value	                                     		   */
/*                                                                                                         */
/*                                                                                                         */
/* Returns:                                                                                                */
/*               E_DRVUART_ERR_PORT_INVALID         												       */
/*               E_DRVUART_ERR_PARITY_INVALID                    										   */
/*               E_DRVUART_ERR_DATA_BITS_INVALID               											   */
/*               E_DRVUART_ERR_STOP_BITS_INVALID                										   */
/*               E_DRVUART_ERR_TRIGGERLEVEL_INVALID      												   */
/*               E_DRVUART_ERR_ALLOC_MEMORY_FAIL                 										   */
/*               E_SUCCESS                                                                          	   */
/*                                                                                                         */
/* Description:                                                                                            */
/*               The function is used to initialize UART. It consists of baud-rate, parity, data-bits,     */
/*               stop-bits, rx-trigger-level and timeout interval settings.                                */
/*                                                                                                         */
/*---------------------------------------------------------------------------------------------------------*/
unsigned int UART_Open(E_UART_PORT u32Port, STR_UART_T *sParam)
{
	UART_T * tUART;
/*
    // Check UART port
    if ((u32Port != UART_PORT0) && (u32Port != UART_PORT1) && (u32Port != UART_PORT2))
    {
		return E_DRVUART_ERR_PORT_INVALID;
	}
    // Check the supplied parity
    if ((sParam->u8cParity != DRVUART_PARITY_NONE) &&
        (sParam->u8cParity != DRVUART_PARITY_EVEN) &&
        (sParam->u8cParity != DRVUART_PARITY_ODD)  &&
        (sParam->u8cParity != DRVUART_PARITY_MARK) &&
        (sParam->u8cParity != DRVUART_PARITY_SPACE))
    {
 		return E_DRVUART_ERR_PARITY_INVALID;		
    }
    // Check the supplied number of data bits
    else if ((sParam->u8cDataBits != DRVUART_DATABITS_5) &&
             (sParam->u8cDataBits != DRVUART_DATABITS_6) &&
             (sParam->u8cDataBits != DRVUART_DATABITS_7) &&
             (sParam->u8cDataBits != DRVUART_DATABITS_8))
    {
 		return E_DRVUART_ERR_DATA_BITS_INVALID;	 
    }
    // Check the supplied number of stop bits
    else if ((sParam->u8cStopBits != DRVUART_STOPBITS_1) &&
             (sParam->u8cStopBits != DRVUART_STOPBITS_2) &&
             (sParam->u8cStopBits != DRVUART_STOPBITS_1_5)
             )
    {
      	return E_DRVUART_ERR_STOP_BITS_INVALID;	 
    }
    // Check the supplied nember of trigger level bytes
    else if ((sParam->u8cRxTriggerLevel != DRVUART_FIFO_1BYTES) &&
             (sParam->u8cRxTriggerLevel != DRVUART_FIFO_4BYTES) &&
             (sParam->u8cRxTriggerLevel != DRVUART_FIFO_8BYTES) &&
             (sParam->u8cRxTriggerLevel != DRVUART_FIFO_14BYTES)&&
             (sParam->u8cRxTriggerLevel != DRVUART_FIFO_30BYTES)&&
             (sParam->u8cRxTriggerLevel != DRVUART_FIFO_46BYTES)&&
             (sParam->u8cRxTriggerLevel != DRVUART_FIFO_62BYTES))
    {
		return E_DRVUART_ERR_TRIGGERLEVEL_INVALID;		 
    }				
*/
	if(u32Port == UART_PORT0){
		SYS->IPRSTC2.UART0_RST = 1;		// Reset IP
		SYS->IPRSTC2.UART0_RST = 0;

	    SYSCLK->APBCLK.UART0_EN = 1;	// Enable UART clock
	}
	else if(u32Port == UART_PORT1){
		SYS->IPRSTC2.UART1_RST = 1;		// Reset IP
		SYS->IPRSTC2.UART1_RST = 0;
		
	    SYSCLK->APBCLK.UART1_EN = 1;	// Enable UART clock
	}
	else{
		SYS->IPRSTC2.UART2_RST = 1;		// Reset IP
		SYS->IPRSTC2.UART2_RST = 0;
		
	    SYSCLK->APBCLK.UART2_EN = 1;	// Enable UART clock
	}
	
	tUART = (UART_T *)((unsigned int)UART0 + u32Port); 

	// Tx FIFO Reset & Rx FIFO Reset & FIFO Mode Enable
  	tUART->FCR.TFR =1;
  	tUART->FCR.RFR =1;

	// Set Rx Trigger Level
	tUART->FCR.RFITL = sParam->u8cRxTriggerLevel;  
	
	// Set Parity & Data bits & Stop bits
	tUART->LCR.SPE	=((sParam->u8cParity)&0x4)?1:0;
	tUART->LCR.EPE	=((sParam->u8cParity)&0x2)?1:0;
	tUART->LCR.PBE	=((sParam->u8cParity)&0x1)?1:0;
		
	tUART->LCR.WLS	=sParam->u8cDataBits;
	tUART->LCR.NSB	=sParam->u8cStopBits;
		
	// Set Time-Out
	tUART->TOR.TOIC	=sParam->u8TimeOut;

    // Check Clock Source
    if(SYSCLK->PWRCON.XTL12M_EN)    	SYSCLK->CLKSEL1.UART_S = 0;
    else if(SYSCLK->PWRCON.OSC22M_EN) 	SYSCLK->CLKSEL1.UART_S = 3;
    else if(SYSCLK->PLLCON.PD==0)    	SYSCLK->CLKSEL1.UART_S = 1;
    else						    	return E_DRVUART_ERR_OPEN;
 
	// Set BaudRate
    BaudRateCalculator(GetUartClk(), sParam->u32BaudRate, u32Port);

	return E_SUCCESS;
}
#if 0
/*---------------------------------------------------------------------------------------------------------*/
/* Function:     DrvUART_SetRTS				                                                               */
/*                                                                                                         */
/* Parameter:        																					   */	
/*				 u32Port 		-[in]   UART Channel:  UART_PORT0 / UART_PORT1                             */
/*               uint8_t	   	-[in]   RTS Value 					          	         				   */
/*                           Set 0: Drive RTS pin to logic 1 (If the LEV_RTS set to low level triggered).  */
/*                                  Drive RTS pin to logic 0 (If the LEV_RTS set to high level triggered)  */
/*                           Set 1: Drive RTS pin to logic 0 (If the LEV_RTS set to low level triggered)   */
/*                                   Drive RTS pin to logic 1 (If the LEV_RTS set to high level triggered) */
/*                           Note. Lev_RTS is RTS Trigger Level. 0 is low level and 1 is high level        */
/*                                                                                                         */
/*               u16TriggerLevel - [in]   RTS Trigger Level :DRVUART_FIFO_1BYTES to DRVUART_FIFO_62BYTES   */
/* Returns:                                                                                                */
/*               None                                                                                      */
/*                                                                                                         */
/* Description:                                                                                            */
/*               The function is used to set RTS information	                                           */
/*---------------------------------------------------------------------------------------------------------*/

void SetRTS(E_UART_PORT u32Port,uint8_t u8Value,uint16_t u16TriggerLevel)
{
	UART_T * tUART;

	tUART = (UART_T *)((unsigned int)UART0 + u32Port);  
	tUART->MCR.RTS = u8Value;
	tUART->FCR.RTS_TRI_LEV = u16TriggerLevel;
}	

/*---------------------------------------------------------------------------------------------------------*/
/* Function:     DrvUART_SetFnRS485                                                                        */
/*                                                                                                         */
/* Parameter:                                                                                              */
/*				 u32Port   	   	  	  -[in]   UART Channel:  UART_PORT0 / UART_PORT1 / UART_PORT2          */
/*               STR_RS485_T   		  -[in]   Ther stucture of RS485  	  	         		               */
/*                                            It includes of                                               */
/*                                               u8cModeSelect: MODE_RS485_AUD / MODE_RS485_AAD            */
/*                                                              MODE_RS485_NMM                             */
/*                                               u8cAddrEnable: Enable or Disable RS-485 Address Detection */
/*                                               u8cAddrValue:  Match Address Value                        */
/*                                               u8cDelayTime:  Set transmit delay time value              */
/*                                               u8cRxDisable:  Enable or Disable receiver function        */
/* Returns:                                                                                                */
/*               None                                                                               	   */
/*                                                                                                         */
/* Description:                                                                                            */
/*               The function is to Set RS485 Control Register                                             */
/*---------------------------------------------------------------------------------------------------------*/
void SetFnRS485(E_UART_PORT u32Port,STR_RS485_T *str_RS485)
{
	UART_T * tUART;

	tUART = (UART_T *)((uint32_t)UART0 + u32Port);  

	tUART->FUNSEL.FUN_SEL 			= FUN_RS485;					
	tUART->ALTCON.RS485_ADD_EN	    = (str_RS485-> u8cAddrEnable) ?1:0;
	tUART->ALTCON.ADDR_MATCH		= str_RS485-> u8cAddrValue ;
	tUART->ALTCON.RS485_NMM		    = (str_RS485-> u8cModeSelect & MODE_RS485_NMM)?1:0;	
	tUART->ALTCON.RS485_AAD		    = (str_RS485-> u8cModeSelect & MODE_RS485_AAD)?1:0;	
	tUART->ALTCON.RS485_AUD		    = (str_RS485-> u8cModeSelect & MODE_RS485_AUD)?1:0;	
	tUART->TOR.DLY                  = str_RS485-> u8cDelayTime;
	tUART->FCR.RX_DIS               = (str_RS485-> u8cRxDisable) ?1:0;
}
#endif
