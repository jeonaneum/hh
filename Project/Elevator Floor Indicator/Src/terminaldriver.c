/*****************************************************************************
*
* Copyright (C) 2003 Atmel Corporation
*
* File			: TerminalDriver.c
* Compiler		: AVR Studio 4.13 / WinAVR_20070525
* Last modified	: 2008.02.20 by JNY
* Description	: Terminal emulation interface
*
****************************************************************************/

#include <stdio.h>
#include "TerminalDriver.h"
#include "NUC1xx.h"
#include "uart.h"
#include "global.h"
#include "main.h"
#include "DrvGPIO.h"
//***************************
// Macros
//***************************
/*** Send escape sequence start ***/
#define SENDESC		Term_Send( 0x1b ); Term_Send( '[' );


//void Term_Initialise();

u08 Term_Get(void);
void Term_Send( u08 data );
void Term_Send_Value_as_Digits( u08 value );

//void Term_Erase_ScreenTop(void);
//void Term_Erase_ScreenBottom(void);
//void Term_Erase_to_Start_of_Line(void);
//void Term_Erase_to_End_of_Line(void);
//void Term_Erase_Line(void);
void Term_Erase_Screen(void);

//void Term_Set_Display_Attribute_Mode( u08 mode );
//void Term_Set_Display_Colour( u08 fg_bg, u08 colour );

void Term_Set_Cursor_Position( u08 row, u08 column );
void Term_Move_Cursor( u08 distance, u08 direction );
void Term_New_Line(void);
//void Term_Save_Cursor_Position();
//void Term_Restore_Cursor_Position(void);

//void Term_Set_Scroll_Mode_All(void);
//void Term_Set_Scroll_Mode_Limit( u08 start, u08 end );
//void Term_Print_Screen(void);


// Initialize UART Terminal
/*
void Term_Initialise(void)
{
 	UCSRA = (1<<U2X);								// enable double speed
	UCSRB = (1<<RXEN)|(1<<TXEN);					// enable RxD,TxD and interrupt
	UCSRC = (1<<URSEL)|(1<<UCSZ1)|(1<<UCSZ0);		// no parity, 1 stop, 8 data

	UBRRH = 0;
	UBRRL = 47;										// baudrate 19200bps
	
	Term_Set_Display_Attribute_Mode( MODE_NONE );	// Disable all previous modes
	Term_Erase_Screen();							// Clear screen
	Term_Set_Cursor_Position( 1, 1 );				// Move to top-left corner
}
*/

// Receive one byte
u08 Term_Get(void)
{
	while(1){
		if(UART1->FSR.TX_EMPTY == 0x00)			// Wait for receive complete
			return ((unsigned char)UART1->DATA );	// Return byte
	}
}

// Transmit one byte
void Term_Send( u08 data )
{
	//RS485_EN_HI;
    while(UART1->FSR.TX_EMPTY == 0x00);		//check Tx Empty

	UART1->DATA = data;							// Send byte
	if(data == '\n'){
        while ((UART1->FSR.TX_EMPTY) == 0x00);	//check Tx Empty
	    UART1->DATA = '\r';						// Send byte
	}
	
	if(tx1.busy_flag == 0){
#ifdef RS485_EN_HI
		RS485_EN_HI;
//		printf("RS485_EN_HI!! -> %d", GPB_14);
#endif
		tx1.busy_flag = 1;
		
		UART1->IER.THRE_IEN = 1;
		tx1.buff_cnt &= UARTBUFMASK;
	}
}

// Convert byte to 3 ASCII digits and send
void Term_Send_Value_as_Digits( u08 value )
{
	u08 digit;
	
	digit = '0';
	while( value >= 100 )				// Still larger than 100 ?
	{
		digit++;						// Increment first digit
		value -= 100;
	}
	
	Term_Send( digit );					// Send first digit
	
	digit = '0';
	while( value >= 10 )				// Still larger than 10 ?
	{
		digit++;						// Increment second digit
		value -= 10;
	}
	
	Term_Send( digit );					// Send second digit
	
	Term_Send( '0' + value );			// Send third digit
}
/*
////////////////////////////////////////////////////////////////////////////////////////////////////
// Send 'clear top of screen' sequence
void Term_Erase_ScreenTop(void)
{
	SENDESC								// Send escape sequence start
	Term_Send( '1' );
	Term_Send( 'J' );
}

// Send 'clear bottom of screen' sequence
void Term_Erase_ScreenBottom(void)
{
	SENDESC
	Term_Send( 'J' );
}

// Send 'clear start of line' sequence
void Term_Erase_to_Start_of_Line(void)
{
	SENDESC
	Term_Send( '1' );
	Term_Send( 'K' );
}

// Send 'clear end of line' sequence
void Term_Erase_to_End_of_Line(void)
{
	SENDESC
	Term_Send( 'K' );
}

// Send 'clear line' sequence
void Term_Erase_Line(void)
{
	SENDESC
	Term_Send( '2' );
	Term_Send( 'K' );
}
*/
// Send 'clear screen' sequence
void Term_Erase_Screen(void)
{
	SENDESC
	Term_Send( '2' );
	Term_Send( 'J' );
}
////////////////////////////////////////////////////////////////////////////////////////////////////
// Set text mode
/*
void Term_Set_Display_Attribute_Mode( u08 mode )
{
	SENDESC
	Term_Send( mode );
	Term_Send( 'm' );
}

// Set text colour
void Term_Set_Display_Colour( u08 fg_bg, u08 colour )
{
	SENDESC
	Term_Send( fg_bg );						// Select foreground/background
	Term_Send( colour );
	Term_Send( 'm' );
}
*/
////////////////////////////////////////////////////////////////////////////////////////////////////
// Set cursor position
// Top-left is (1,1)
void Term_Set_Cursor_Position( u08 row, u08 column )
{
//	printf("Term_Set_Cursor_Position(row=%d, col=%d)\r\n",row, column);
	//SENDESC
	Term_Send( 27 ); Term_Send( '[' );
	Term_Send_Value_as_Digits( row );		// Convert row byte
	Term_Send( ';' );
	Term_Send_Value_as_Digits( column );	// Convert column byte
	Term_Send( 'H' );
}

// Move cursor
void Term_Move_Cursor( u08 distance, u08 direction )
{
	//SENDESC
	Term_Send( 27 ); Term_Send( '[' );
	Term_Send_Value_as_Digits( distance );	// Convert distance byte
	Term_Send( direction );
}

void Term_New_Line(void)
{
	Term_Send( '\n' );
	Term_Send( '\r' );
}
/*
// Save cursor position
void Term_Save_Cursor_Position(void)
{
	SENDESC
	Term_Send( 's' );
}

// Restore cursor position
void Term_Restore_Cursor_Position(void)
{
	SENDESC
	Term_Send( 'u' );
}
*/
////////////////////////////////////////////////////////////////////////////////////////////////////
/*
// Enable scroll for entire screen
void Term_Set_Scroll_Mode_All(void)
{
	SENDESC
	Term_Send( 'r' );
}

// Limit screen scrolling to some lines only
void Term_Set_Scroll_Mode_Limit( u08 start, u08 end )
{
	SENDESC
	Term_Send_Value_as_Digits( start );		// Convert start line byte
	Term_Send( ';' );
	Term_Send_Value_as_Digits( end );		// Convert end line byte
	Term_Send( 'r' );
}

// Send 'print screen' command sequence
void Term_Print_Screen(void)
{
	SENDESC
	Term_Send( 'i' );
}
*/
