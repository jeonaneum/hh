#ifndef _MAIN_H_
#define _MAIN_H_

#include "global.h"


//----------------------------------------------------------------------------------------------------------
//	GPIO definition name  		  PIN / MFP0 / MFP1       / MFP2
//----------------------------------------------------------------------------------------------------------
enum	// PA pin
{
	CTRLER_BUZ = 0,				// 44 / PA0  / ADC0						// (O)
	CTRLER_ADD,					// 45 / PA1  / ADC1						// (O)
	CTRLER_LINK,				// 46 / PA2  / ADC2						// (I)
	CTRLER_SET,					// 47 / PA3  / ADC3						// (I)
	RIGHT,						// 48 / PA4  / ADC4						// (I)
	DOWN,						// 49 / PA5  / ADC5						// (I)
	LEFT,						// 50 / PA6  / ADC6						// (I)
	UP,							// 51 / PA7  / ADC7						// (I)
	I2C0_SDA,					// 09 / PA8  / I2C0_SDA				// (OD)
	I2C0_SCL,					// 08 / PA9  / I2C0_SCL				// (OD)
	H_SYNC,						// 07 / PA10 / I2C1_SDA				// (I)
	V_SYNC,						// 06 / PA11 / I2C1_SCL				// (I)
	CTRLER_RESET,				// 40 / PA12 / PWM0					// (I)
	CTRLER_FD0,					// 39 / PA13 / PWM1					// (O)
	CTRLER_FD1,					// 38 / PA14 / PWM2					// (O)
	CTRLER_FD2,					// 37 / PA15 / PWM3	/ I2SMCLK// (O)
};

enum	//PB pin
{
	DBGRXD = 0,					// 21 / PB0  / RX0					// (OD)
	DBGTXD,						// 22 / PB1  / TX0					// (OD)
	V_LOSS,						// 23 / PB2  / RTS0					// (I)
	CTRLER_FND_CH5,				// 24 / PB3  / CTS0					// (O)
	RS485_RXD,					// 10 / PB4  / RX1					// (OD) 
	RS485_TXD,					// 11 / PB5  / TX1					// (OD) 
	RS485_EN,					// 12 / PB6  / RTS1					// (O)
	LASER,						// 13 / PB7  / CTS1					// (O)
	UCC_Data_EN,				// 64 / PB8  / STADC				// (O)
	PHOTO_SENSOR_L1,			// 32 / PB9  / TM1					// (I)
	PHOTO_SENSOR_L2,			// 31 / PB10 / TM2					// (I)
	PROX_SENSOR_L3A,			// 30 / PB11 / TM3 / PWM4		// (I)
	VID_FMT0,					// 03 / PB12 / CPO1					// (I)
	VID_FMT1,					// 02 / PB13 / CPO0	 / CLKO	// (I)
	VID_FMT2,					// 01 / PB14 / INT0					// (I)
//	10K,						// 57 / PB15 / INT1					// (I)
};

enum	//PC pin
{
	CTRLER_FND_CH1 = 0,			// 28 / PC0  / SPI_CS00	  / I2SLRCLK	// (O)
	CTRLER_FND_CH2,				// 27 / PC1  / SPI_SCLK00 / I2SBCLK	// (O)
	CTRLER_FND_CH3,				// 26 / PC2  / SPI_MISO00 / I2SDI		// (O)
	CTRLER_FND_CH4,				// 25 / PC3  / SPI_MOSI00 / I2SDO		// (O)
	LASER_SW = 6,				// 54 / PC6  / CPP0   							// (I)
	OSD_SW,						// 53 / PC7  / CPN0   							// (I)
	CTRLER_FD3,					// 36 / PC8  / SPI_CS10   					// (O)
	BD_REV1,					// 35 / PC9  / SPI_SCLK10   				// (I)
	BD_REV2,					// 34 / PC10 / SPI_MISO10					// (I)
	BD_REV3,					// 33 / PC11 / SPI_MOSI10					// (I)
	UTC_VIDEO_SEL1 = 14,		// 56 / PC14 / CPP1								// (I)
	UTC_VIDEO_SEL2,				// 55 / PC15 / CPN1								// (I)
};

enum	//PE pin
{
	PROX_SENSOR_L4A = 5,		//29 / PE5 / PWM5							// (I)
};

/*
//	GPIO SET IN_OUT
#define CTRLER_BUZ_PIN_OUT_SET			DrvGPIO_Open(E_GPA, CTRLER_BUZ, E_IO_OUTPUT)	
#define CTRLER_ADD_PIN_OUT_SET			DrvGPIO_Open(E_GPA, CTRLER_ADD, E_IO_OUTPUT)	
#define CTRLER_RESET_PIN_OUT_SET		DrvGPIO_Open(E_GPA, CTRLER_RESET, E_IO_INPUT)
#define CTRLER_LINK_PIN_OUT_SET			DrvGPIO_Open(E_GPA, CTRLER_LINK, E_IO_INPUT)	
#define CTRLER_FD0_PIN_OUT_SET			DrvGPIO_Open(E_GPA, CTRLER_FD0, E_IO_OUTPUT)	
#define CTRLER_FD1_PIN_OUT_SET			DrvGPIO_Open(E_GPA, CTRLER_FD1, E_IO_OUTPUT)	
#define CTRLER_FD2_PIN_OUT_SET			DrvGPIO_Open(E_GPA, CTRLER_FD2, E_IO_OUTPUT)	

#define CTRLER_FND_CH5_PIN_OUT_SET		DrvGPIO_Open(E_GPB, CTRLER_FND_CH5, E_IO_OUTPUT)	
#define LASER_PIN_OUT_SET				DrvGPIO_Open(E_GPB, LASER, E_IO_OUTPUT)
#define UCC_Data_EN_PIN_OUT_SET			DrvGPIO_Open(E_GPB, UCC_Data_EN, E_IO_OUTPUT)
#define RS485_EN_PIN_OUT_SET			DrvGPIO_Open(E_GPB, RS485_EN, E_IO_OUTPUT)

#define CTRLER_FND_CH1_PIN_OUT_SET		DrvGPIO_Open(E_GPC, CTRLER_FND_CH1, E_IO_OUTPUT)
#define CTRLER_FND_CH2_PIN_OUT_SET		DrvGPIO_Open(E_GPC, CTRLER_FND_CH2, E_IO_OUTPUT)
#define CTRLER_FND_CH3_PIN_OUT_SET		DrvGPIO_Open(E_GPC, CTRLER_FND_CH3, E_IO_OUTPUT)
#define CTRLER_FND_CH4_PIN_OUT_SET		DrvGPIO_Open(E_GPC, CTRLER_FND_CH4, E_IO_OUTPUT)
#define CTRLER_FD3_PIN_OUT_SET				DrvGPIO_Open(E_GPC, CTRLER_FD3, E_IO_OUTPUT)

#define RIGHT_PIN_IN_SET				DrvGPIO_Open(E_GPA, RIGHT, E_IO_INPUT)			
#define DOWN_PIN_IN_SET					DrvGPIO_Open(E_GPA, DOWN, E_IO_INPUT)		
#define LEFT_PIN_IN_SET					DrvGPIO_Open(E_GPA, LEFT, E_IO_INPUT)		
#define UP_PIN_IN_SET					DrvGPIO_Open(E_GPA, UP, E_IO_INPUT)
#define H_SYNC_PIN_IN_SET				DrvGPIO_Open(E_GPA, H_SYNC, E_IO_INPUT)		
#define V_SYNC_PIN_IN_SET				DrvGPIO_Open(E_GPA, V_SYNC, E_IO_INPUT)		

#define V_LOSS_PIN_IN_SET				DrvGPIO_Open(E_GPB, V_LOSS, E_IO_INPUT)
#define PHOTO_SENSOR_L1_PIN_IN_SET		DrvGPIO_Open(E_GPB, PHOTO_SENSOR_L1, E_IO_INPUT)	
#define PHOTO_SENSOR_L2_PIN_IN_SET		DrvGPIO_Open(E_GPB, PHOTO_SENSOR_L2, E_IO_INPUT)	
#define PROX_SENSOR_L3A_PIN_IN_SET		DrvGPIO_Open(E_GPB, PROX_SENSOR_L3A, E_IO_INPUT)
#define VID_FMT0_PIN_IN_SET				DrvGPIO_Open(E_GPB, VID_FMT0, E_IO_INPUT)
#define VID_FMT1_PIN_IN_SET				DrvGPIO_Open(E_GPB, VID_FMT1, E_IO_INPUT)
#define VID_FMT2_PIN_IN_SET				DrvGPIO_Open(E_GPB, VID_FMT2, E_IO_INPUT)

#define LASER_SW_PIN_IN_SET				DrvGPIO_Open(E_GPC, LASER_SW, E_IO_INPUT)
#define OSD_SW_PIN_IN_SET				DrvGPIO_Open(E_GPC, OSD_SW, E_IO_INPUT)
#define BD_REV1_PIN_IN_SET				DrvGPIO_Open(E_GPC, BD_REV1, E_IO_OUTPUT)
#define BD_REV2_PIN_IN_SET				DrvGPIO_Open(E_GPC, BD_REV2, E_IO_INPUT)
#define BD_REV3_PIN_IN_SET				DrvGPIO_Open(E_GPC, BD_REV3, E_IO_INPUT)
#define UTC_VIDEO_SEL1_PIN_IN_SET		DrvGPIO_Open(E_GPC, UTC_VIDEO_SEL1, E_IO_INPUT)
#define UTC_VIDEO_SEL2_PIN_IN_SET		DrvGPIO_Open(E_GPC, UTC_VIDEO_SEL2, E_IO_INPUT)
*/			
//==========================================================================
#define RS485_EN_HI						DrvGPIO_SetBit(E_GPB, RS485_EN)
#define RS485_EN_LO						DrvGPIO_ClrBit(E_GPB, RS485_EN)
//#define PROX_SENSOR_USE

// Controller Define -------------------------------------------------
#define BUZZER_ON						DrvGPIO_ClrBit(E_GPA, CTRLER_BUZ)
#define BUZZER_OFF						DrvGPIO_SetBit(E_GPA, CTRLER_BUZ)
#define FND_BAR_ON						DrvGPIO_ClrBit(E_GPA, CTRLER_ADD)
#define FND_BAR_OFF						DrvGPIO_SetBit(E_GPA, CTRLER_ADD)
#define CODE_DEFAULT					0
#define NO_DATA							10
#define CODE_DELETE						44

#define FND_CH1_ON						DrvGPIO_SetBit(E_GPC, CTRLER_FND_CH1)
#define FND_CH1_OFF						DrvGPIO_ClrBit(E_GPC, CTRLER_FND_CH1)
#define FND_CH2_ON						DrvGPIO_SetBit(E_GPC, CTRLER_FND_CH2)
#define FND_CH2_OFF						DrvGPIO_ClrBit(E_GPC, CTRLER_FND_CH2)
#define FND_CH3_ON						DrvGPIO_SetBit(E_GPC, CTRLER_FND_CH3)
#define FND_CH3_OFF						DrvGPIO_ClrBit(E_GPC, CTRLER_FND_CH3)
#define FND_CH4_ON						DrvGPIO_SetBit(E_GPC, CTRLER_FND_CH4)
#define FND_CH4_OFF						DrvGPIO_ClrBit(E_GPC, CTRLER_FND_CH4)
#define FND_CH5_ON						DrvGPIO_SetBit(E_GPB, CTRLER_FND_CH5)
#define FND_CH5_OFF						DrvGPIO_ClrBit(E_GPB, CTRLER_FND_CH5)


#define FD3_ON							DrvGPIO_SetBit(E_GPC, CTRLER_FD3)
#define FD3_OFF							DrvGPIO_ClrBit(E_GPC, CTRLER_FD3)
#define FD2_ON							DrvGPIO_SetBit(E_GPA, CTRLER_FD2)
#define FD2_OFF							DrvGPIO_ClrBit(E_GPA, CTRLER_FD2)	
#define FD1_ON							DrvGPIO_SetBit(E_GPA, CTRLER_FD1)
#define FD1_OFF							DrvGPIO_ClrBit(E_GPA, CTRLER_FD1)	
#define FD0_ON							DrvGPIO_SetBit(E_GPA, CTRLER_FD0)
#define FD0_OFF							DrvGPIO_ClrBit(E_GPA, CTRLER_FD0)

//#define _720p_use
// fnd ch
enum{
	CH1=0,
	CH2,
	CH3,
	CH4,
	CH5,
};

enum{
	MODE_DEFAULT=0,
	MODE_0_MENU,
	MODE_1_BASE,
	MODE_2_OSD,
	MODE_3_ID,
	MODE_4_RESET,
};

// UART Code Byte Define -----------------------------------------------
#define START_CODE					0xa0
#define DEVICE_CODE					0xEF
#define DEVSUB_CODE					0x04

// Laser Zig Code Define -----------------------------------------------
#define LASER_ZIG_ON				DrvGPIO_SetBit(E_GPB, LASER)
#define LASER_ZIG_OFF				DrvGPIO_ClrBit(E_GPB, LASER)

#define LASER_ON_TIME				1800	// 3min   = 3 X 60 X 100 X 10ms

// UTC -----------------------------------------------
#define TMR_CLR_CEN_IE				0x87FFFF00	// Clear CEN, IE, Operation mode, set Prescale 0
#define TMR_SET_TCMPR				0x00FFFFFF	// Set TCMPR to max value
#define TMR_START_ONESHOT			0x64000000	// Set timer enable, interrupt enable, reset
#define TMR_READY_ONESHOT			0x24000000	// Set interrupt enable, reset
#define TMR_READY_PERIOD			0x0C010000	// timer enable, period mode, reset, tdr_en
#define TMR_MODE					(3<<27)
#define MODE_ONE_SHOT				(0<<27)
#define setTMR1_ONESHOT				{TCSR1 &= ~TMR_MODE; TCSR1 |= MODE_ONE_SHOT ;}

#define TCMPR1						TIMER1->TCMPR
#define TCSR1						TIMER1->u32TCSR

#define UTC_OFF						DrvGPIO_SetBit(E_GPB, UCC_Data_EN)
#define UTC_ON						DrvGPIO_ClrBit(E_GPB, UCC_Data_EN)
#define UTC_EN_PIN					DrvGPIO_GetPortBits(E_GPB)
	
#define AHD_ON_HSYNC_CNT			13
#define AHD_OFF_HSYNC_CNT			18
#define CVI_ON_HSYNC_CNT			15
#define CVI_OFF_HSYNC_CNT			23
#define TVI_ON_HSYNC_CNT			7
#define TVI_OFF_HSYNC_CNT			12
#define CVBS_ON_HSYNC_CNT			8192
#define CVBS_OFF_HSYNC_CNT			0

#define NOVID_OFF_HSYNC_CNT			(6)
#define MAX_OFF_HSYNC_CNT			(25)
#define CHK_CEN_HSYNC_CNT			(MAX_OFF_HSYNC_CNT+2)
#define CHK_END_HSYNC_CNT			(CHK_CEN_HSYNC_CNT+10)

#define INT_ON_HSYNC_DLY			160	//(__USE_SYS_CLK__/70000)
#define DEFAULT_MODE				NONE
#define DEFAULT_FRMT				in_1920x1080p30
#define _USE_H_TIMING_CHK_			1	// 0:no chk, 1:int, 2:get MCU1
#define _USE_ON_OFF_VINT_			1	// 0:always on, 1:onoff control
#define _MODE_SET_FUNC_				0	// 0:dip_sw, 1: uart

#define DISABLE_VINT					GPIOA->IEN &= ~IF_EN11//DrvGPIO_DisableInt(E_GPA, V_SYNC);
#define ENABLE_VINT						GPIOA->IEN |= IF_EN11//DrvGPIO_EnableInt(E_GPA, V_SYNC, E_IO_FALLING, E_MODE_EDGE);
	
#define DISABLE_HINT					GPIOA->IEN &= ~IF_EN10//DrvGPIO_DisableInt(E_GPA, H_SYNC);
#define ENABLE_HINT						GPIOA->IEN |= IF_EN10//DrvGPIO_EnableInt(E_GPA, H_SYNC, E_IO_FALLING, E_MODE_EDGE);

enum {
	in_ntsc = 0,
	in_pal,
	in_1280x720p60,
	in_1280x720p50,
	in_1280x720p30,
	in_1280x720p25,
	in_1920x1080p30,
	in_1920x1080p25,
	in_unknown
};

enum{
	RESOLUTION_PAL=0,
	RESOLUTION_NTSC,
	RESOLUTION_HD25P,
	RESOLUTION_HD30P,
	RESOLUTION_FULLHD25P,
	RESOLUTION_FULLHD30P,
	RESOLUTION_3MP,
	RESOLUTION_4MP,
};

enum{
	RESOLUTION_NONE = 0,
	SD_PAL,
	SD_NTSC,
	HD_720P25,
	HD_720P30,
	HD_720P50,
	HD_720P60,
	HD_1080P25,
	HD_1080P30,
	UHD_3MP,
	UHD_4MP,
};
enum {
	CVBS=0,
	TVI,
	CVI,
	AHD,
};

typedef struct {
	unsigned int V_Stat;
	unsigned int V_Err;
	unsigned int V_ErrCnt;
	#ifdef _DEBUG_
	unsigned int V_Cnt_Buff;
	#endif
	unsigned int V_Cnt;
	unsigned int H_Cnt;
	unsigned int H_CntDly;
	unsigned int H_ChkDly;
	unsigned int H_ChkDlyCnt;
	unsigned int H_ChkDlyBuf;
}SYNC_CHK_ST;

typedef struct {
	SYNC_CHK_ST  sync;
	unsigned int TDR;
	unsigned int VideoType;
	unsigned int VideoFmt;
	unsigned int En_Start;
	unsigned int On_Cnt;
	unsigned int Off_Cnt;
}UTC_CTRL_ST;

// Key Code Define -----------------------------------------------
#define KEY_STABLE_TIME				5
#define KEY_REPEAT_TIME				350

enum{
	KEY_NONE=0,
	KEY_SET=0x01,
	KEY_RIGHT=0x02,
	KEY_DOWN=0x04,
	KEY_UP=0x08,
	KEY_LEFT=0x10,
	KEY_LASER=0x20,			// SW1
	KEY_OSD=0x40,			// SW2
	KEY_MODE=0x80,
};

// Elevator Status Code Byte Define -----------------------------------------------
#define STOP_CHK_TIME				40
#define MOVE_CHK_TIME				30
#define RESET_CHK_TIME				100		// 1sec
enum{
	RULE_NONE=0,
	RULE_STOP,
	RULE_MOVE,
};

enum{
	MOVE_NONE=0,
	MOVE_UP,
	MOVE_DOWN,
	MOVE_ING_UP,
	MOVE_ING_DOWN,
	MOVE_RESET,
	MOVE_ING,
};


#define BASEMENT					0
#define OSD_TEN_SIZE				12
#define OSD_ONE_SIZE				11

// Floor Information
typedef struct{
	unsigned char max;			//maximum floor
	unsigned char min;				//minimum floor
	unsigned char base;			//base floor
	unsigned char current;		//current floor info
	unsigned char upper;			//upper floor info
	unsigned char lower;			//lower floor info	
}FloorInfo_t;

// Serial Menu
#define	MENU_COL			10
#define	MENU_ROW			6
#define	RUN_MODE			1
#define	SETUP_MODE			2

enum{
	SERIAL_MENU=0,
	SERIAL_MENU_1,
	SERIAL_MENU_2,
	SERIAL_MENU_3,
};

//#define _DBG_PRINT_
//---------------------------------------------------------------------
extern FloorInfo_t Floor;
extern u08 osd_position;
extern u08 rs485_id[3];
extern u08 osd_floor[110][2];
extern u08 osd_char_code[110];
extern u32 door_max;
extern u32 door_min;
extern u08 video_signal;
//==========================================================================
void GPIO_Init(void);
void System_Reset(void);
void Send_Data(u08, u32);
void Vsync_Chk_Proc(void);
void Floor_Display(u08);
void Floor_Reset_Display(void);
void test_FloorDisplay(void);
void Osd_Init(void);
void Controller_Mode_Init(void);
void Contoller_Key_Proc(u08);
void Controller_Init(void);
void Default_FloorInfo_to_OSDInfo(u08);
void Fnd_Set(u08, u08, u08, u08);
void Display_ID(void);
void Set_Sync_Int(u08, u08);
void CFG_Save_Sync_Off(u08);
void SOH701_Reset(void);
void UTC_Init(void);
void Test_Key_Proc(void);
void Display_TEST(void);
void Test_Floor_Display(u08 pos);
void Rs485_Send(u08 mode);
//==========================================================================
#endif




