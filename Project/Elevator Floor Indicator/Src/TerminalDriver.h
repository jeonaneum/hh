
#ifndef TERMINALDRIVER_H
#define TERMINALDRIVER_H

//***************************
// Misc definitions
//***************************

/*** Text modes ***/
#define MODE_NONE         '0'
#define MODE_BOLD         '1'
#define MODE_DIM          '2'
#define MODE_UNDERLINE    '4'
#define MODE_BLINK        '5'
#define MODE_REVERSED     '7'
#define MODE_CONCEALED    '8'


/*** Text colours ***/
#define COL_FOREGROUND    '3'
#define COL_BACKGROUND    '4'

#define COL_BLACK         '0'
#define COL_RED           '1'
#define COL_GREEN         '2'
#define COL_YELLOW        '3'
#define COL_BLUE          '4'
#define COL_MAGENTA       '5'
#define COL_CYAN          '6'
#define COL_WHITE         '7'


/*** Cursor move direction ***/
/*
#define MOVE_UP           'A'
#define MOVE_DOWN         'B'
#define MOVE_RIGHT        'C'
#define MOVE_LEFT         'D'
*/


//***************************
// Function prototypes
//***************************

//extern void Term_Initialise();

extern unsigned char Term_Get(void);
extern void Term_Send( unsigned char data );
extern void Term_Send_Value_as_Digits( unsigned char value );

//extern void Term_Erase_ScreenTop(void);
//extern void Term_Erase_ScreenBottom(void);
//extern void Term_Erase_to_Start_of_Line(void);
//extern void Term_Erase_to_End_of_Line(void);
//extern void Term_Erase_Line(void);
extern void Term_Erase_Screen(void);

//extern void Term_Set_Display_Attribute_Mode( unsigned char mode );
//extern void Term_Set_Display_Colour( unsigned char fg_bg, unsigned char colour );

extern void Term_Set_Cursor_Position( unsigned char row, unsigned char column );
extern void Term_Move_Cursor( unsigned char distance, unsigned char direction );
extern void Term_New_Line(void);
//extern void Term_Save_Cursor_Position();
//extern void Term_Restore_Cursor_Position(void);

//extern void Term_Set_Scroll_Mode_All(void);
//extern void Term_Set_Scroll_Mode_Limit( unsigned char start, unsigned char end );
//extern void Term_Print_Screen(void);

#endif
