//-----------------------------------------------------------------------------------//
//	# Project NAME	: SC-EFI04
//	# FILE NAME		: SC-EFI04.c
//	# AUTHOR		: JHH
//	# DATE			: 2017-07-20
//	# DESCRIPTION	: Elevator Floor Indicator

#include <stdio.h>
#include <string.h>
#include "NUC1xx.h"
#include "common.h"
#include "global.h"
#include "main.h"
#include "Version.h"
#include "uart.h"
#include "i2c.h"
#include "DrvSYS.h"
#include "DrvGPIO.h"
#include "FMC.h"
#include "SOH701.h"
#include "terminaldriver.h"
#include "adc.h"

//------------------------------------------------------------------
#ifdef __USE_WDT__
unsigned int WdtRstFlag;
#endif

// related Timer
u08 tFlag_1ms = 0;
u08 tCnt_1ms = 0;
u08 tFlag_10ms = 0;
u08 tCnt_10ms = 0;
u08 tFlag_50ms = 0;
u08 tCnt_50ms = 0;
u08 tFlag_100ms = 0;
u08 tCnt_100ms = 0;
u08 tFlag_200ms = 0;
u08 tCnt_200ms = 0;
u08 tFlag_1sec = 0;
u08 tCnt_1sec = 0;
u08 tFlag_3sec = 0;
u32 tCnt_Srv = 0;
u08 Fnd_Cnt_1ms = 0;
//-----------------------

// related UTC
u32 Video_Mode = 0;
u32 Video_Frmt = 0;
u32 Utc_On_Cnt = 0;
u32 Utc_Off_Cnt = 0;
u32 UtcEn_Start = 0;

u32 V_Sync_Stat = 0;
u32 V_Sync_Err = 0;
u32 V_Sync_ErrCnt = 0;
u32 V_Sync_Cnt = 0;
#ifdef _DBG_PRINT_
u32 V_Sync_Cnt_Buff = 0;
#endif

u32 H_Sync_CntDly = 0;
u32 H_Sync_Cnt = 0;
u32 H_Sync_ChkDly = 0;
u32 H_Sync_ChkDlyCnt = 0;
u32 H_Sync_ChkDlyBuf = 0;

u08 H_Sync_flag = 0;
u08 V_Sync_flag = 0;

u32 TDR_Buff = 0;
u08 video_format = 0;
u08 video_signal = 0;
u08 pre_video_signal = 0;
u08 pre_video_temp = 0;
u08 comp_cvbs_cnt = 0;

#define TMR1_START_ONESHOT		{TCSR1 &= TMR_CLR_CEN_IE; TCMPR1 = H_Sync_CntDly; 		TCSR1 |= TMR_START_ONESHOT;}
#define TMR1_READY_ONESHOT		{TCSR1 &= TMR_CLR_CEN_IE; TCMPR1 = H_Sync_CntDly; 		TCSR1 |= TMR_READY_ONESHOT;}
#define TMR1_READY_PERIOD			{TCSR1 &= TMR_CLR_CEN_IE; TCMPR1 = TMR_SET_TCMPR; 	TCSR1 |= TMR_READY_PERIOD;}
//-----------------------
// related Laser
static volatile u16 zig_on_count = 0;
static volatile u08 zig_sw_flag = 0;
//-----------------------
// related Key
u16 key_rechk_cnt = 0;
u08 key_comp = 0;
u08 key_rechk_flag = 0;
u08 key_rechk_flag_cnt = 0;

// realted door
u08 door_flag = 0;
u08 door_step = 0;
u08 door_cnt = 0;
u32 door_max = 0;
u32 door_min = 0;
u08 door_data = 0;

// related Photo Sensor
u08 sensor_int_flag = 0;
u08 move_flag = 0;
u08 move_step = 0;
u08 move_stat_chk_flag = 0;
u08 stop_cnt_flag = 0;
u08 reset_cnt_flag = 0;
u08 move_cnt_flag = 0;
u08 stop_cnt = 0;
u08 reset_cnt = 0;
u08 move_cnt = 0;
u08 cur_sensor = 0;
u08 pre_sensor = 0;
u08 rule_flag = 0;
u08 cfg_flag = 0;

//reated Controller
u08 buzzer_cnt_flag = 0;
u08 buzzer_cnt = 0;
u08 ctrl_connect_flag = 0;
u08 fnd_data[5] = {0, };
u08 fnd_ch_cnt = 0;
u08 ctrl_mode = 0;
u08 fnd_bar_flag[5] = {0, };
u08 ctrl_floor_buff = 0;
u08 floor_delete_stat = 0;
u08 fnd_ch_chg_flag = 0;
u08 rs485_id[3] = {0,};
u08 ctrl_reset_flag = 0;
u08 ctrl_menu_flag = 0;
u08 fnd_cursor = CH1;
u08 fnd_num = 0;
u08 fnd_max_cursor = 0;
u08 fnd_min_cursor = 0;
u08 blink_flag[5] = {0,};
u08 blink_cnt[5] = {0,};

//related OSD
u08 osd_boot_cnt = 0;
u08 osd_boot_flag = 0;
u08 row_data = 15;
u08 osd_position = 0;
u08 osd_init_cnt = 0;
u08 osd_flag = 0;
u08 f_char, b_char = 0;
u08 id_flag = 0;
u08 osd_out_flag = 0;
u08 osd_reset_flag = 0;
u08 osd_char_code[110];
FloorInfo_t Floor = {0,};

u08 door_rs_data = 0;
u08 pre_485_stat[8] = {0, };
u08 test_mode = 0;
u08 test_cnt = 0;
u08 test_door = 0;
u08 test_move = 0;
//related Serial_Menu
const u08 Floor_Str[]   = "Floor | ";
const u08 Osd_Str[]     = " OSD  | ";
#ifdef BUGFIX_VERSION_DISPLAY
	const u08 Setup_Str[]   = " Floor Display OSD Setup      ";	// 30
#else
	const u08 Setup_Str[]   = "     SC-EFI04 v1.0  Floor Display OSD Setup     ";	// 48
#endif
const u08 Setup_Line1[] = "================================================";
const u08 ID_str[] = " Device ID : ";
const u08 reset[] = " Reset?  N";
const u08 Setup_Flr00[] = " RST -9  -8  -7  -6  -5  -4  -3  -2  -1 ";			// 40
const u08 Setup_Flr01[] = " 00  01  02  03  04  05  06  07  08  09 ";
const u08 Setup_Flr02[] = " 10  11  12  13  14  15  16  17  18  19 ";
const u08 Setup_Flr03[] = " 20  21  22  23  24  25  26  27  28  29 ";
const u08 Setup_Flr04[] = " 30  31  32  33  34  35  36  37  38  39 ";
const u08 Setup_Flr05[] = " 40  41  42  43  44  45  46  47  48  49 ";
const u08 Setup_Flr06[] = " 50  51  52  53  54  55  56  57  58  59 ";
const u08 Setup_Flr07[] = " 60  61  62  63  64  65  66  67  68  69 ";
const u08 Setup_Flr08[] = " 70  71  72  73  74  75  76  77  78  79 ";
const u08 Setup_Flr09[] = " 80  81  82  83  84  85  86  87  88  89 ";
const u08 Setup_Flr10[] = " 90  91  92  93  94  95  96  97  98  99 ";

const u08 *Setup_Flr[] = {Setup_Flr00,Setup_Flr01,Setup_Flr02,Setup_Flr03,Setup_Flr04,Setup_Flr05,Setup_Flr06,Setup_Flr07,Setup_Flr08,Setup_Flr09,Setup_Flr10};

const u08 Config_New[]  = " [1] Device Setting";
const u08 Config_ID[] = " [2] Device ID Setting";
const u08 Config_Reset[] = " [3] Device Reset";						

const u08 Config_Cmd[]  = " CMD>";											// 04
u08 id_cursor = 0;
u08 SysMode=SERIAL_MENU;

u08 Arrow_Flag = 0;
u08 Save_Flag = 0;

u08 Cursor = 0;
u08 Cursor_Pos_Col = 0;
u08 Cursor_Pos_Row = 0;

unsigned char no_change_flag = 0;
//======================================================================

u08 osd_floor[110][2] = {{0,},};
u08 char_f = 0;
u08 char_b = 0;


//#define TEST_MODE
//----------------------------------------------------------
void MCU_Init(void)
{
	UNLOCKREG();									// Disable Write-Protection [NUC1xx.h] 									

	GPIO_Init();										// GPIO Setting

#if __USE_SYS_CLK__ == __XTAL
	DrvSYS_SetOscCtrl(E_SYS_XTL12M, ENABLE);			// Enable Ex xtal clk
	DrvSYS_SelectPLLSource(E_SYS_EXTERNAL_12M);
#elif __USE_SYS_CLK__ == __IRC22M
	DrvSYS_SetOscCtrl(E_SYS_OSC22M, ENABLE);			// Enable internal 22M
	DrvSYS_SelectPLLSource(E_SYS_INTERNAL_22M);
#endif
	DrvSYS_Open(50000000);
	DrvSYS_SelectSysTickSource(0);						// 0:Ext 12M, 1:Ext 32K, 2:Ext 12M/2, 3:HCLK/2, 7:Int 22M/2
	SysTick->CTRL = 0x1;								// clk src : core clk, counter enable
	DrvSYS_SelectIPClockSource(E_SYS_UART_CLKSRC, 0);	// Select UART Clock Source = 0:Ext 12M, 1:PLL, 3:int 22MHz
	FMC_Enable();

	DrvSYS_EnableHighPerformanceMode();
}

void GPAB_IRQHandler(void)
{
	u32 GPA_int_status;
	u32 GPB_int_status;
	
	GPA_int_status = GPIOA->ISRC;
	GPIOA->ISRC = GPA_int_status;
	GPB_int_status = GPIOB->ISRC;
	GPIOB->ISRC = GPB_int_status;
	
//SENSOR INT	-----------------------------------------------
	if(GET_BIT(GPB_int_status, PHOTO_SENSOR_L1) || GET_BIT(GPB_int_status, PHOTO_SENSOR_L2))
	{
//		printf("sensor int gen \n");
		sensor_int_flag = 1;
	}
	
//Controller Connect INT
/*	
	if(GET_BIT(GPA_int_status, CTRLER_LINK))
	{
		printf("(GET_BIT(GPA_int_status, CTRLER_LINK) : %d \n", (GET_BIT(GPA_int_status, CTRLER_LINK)));
		ctrl_connect_flag = 1;
	}
*/
//UTC INT-----------------------------------------------
	if(GET_BIT(GPA_int_status, H_SYNC))
	{
		H_Sync_Cnt++;
		if(H_Sync_Cnt == Utc_On_Cnt){UTC_ON;}
		else if(H_Sync_Cnt == Utc_Off_Cnt){ UTC_OFF; }
#if _USE_H_TIMING_CHK_ == 1
		else if(H_Sync_Cnt == MAX_OFF_HSYNC_CNT){ TMR1_READY_PERIOD; }
		else if(H_Sync_Cnt == CHK_CEN_HSYNC_CNT){ TIMER1->TCSR.CEN = 1; }		//H_sync high width
		else if(H_Sync_Cnt == CHK_END_HSYNC_CNT){ TDR_Buff = TIMER1->TDR; }
#endif
	}
	
	if(GET_BIT(GPA_int_status, V_SYNC))
	{
		TMR1_START_ONESHOT;
#if _USE_ON_OFF_VINT_
		DISABLE_VINT;
#endif
		V_Sync_Cnt++;
		H_Sync_Cnt = 0;
	}
}

void GPIO_INT_Init(void)
{
	DrvGPIO_EnableDebounce(E_GPA, H_SYNC);
	DrvGPIO_EnableDebounce(E_GPA, V_SYNC);
	
	DrvGPIO_EnableDebounce(E_GPA, CTRLER_LINK);
	
	DrvGPIO_EnableDebounce(E_GPB, PHOTO_SENSOR_L1);
	DrvGPIO_EnableDebounce(E_GPB, PHOTO_SENSOR_L2);
	
	GPIOB->IMD &= (IMD9_EDG|IMD10_EDG);	//9: L1, 10: L2
	GPIOB->IEN |= IR_EN9|IR_EN10|IF_EN9|IF_EN10;
	
	GPIOA->IMD &= (IMD2_EDG|IMD10_EDG|IMD11_EDG);	//2: LINK, 10: H_SYNC, 11: V_SYNC
#if _USE_ON_OFF_VINT_
	GPIOA->IEN = 0;
//	DrvGPIO_DisableInt(E_GPA, H_SYNC);
//	DrvGPIO_DisableInt(E_GPA, V_SYNC);
#else
	GPIOA->IEN |= IF_EN2|IF_EN11;
//	DrvGPIO_EnableInt(E_GPA, V_SYNC, E_IO_FALLING, E_MODE_EDGE);		// v-sync falling, h-sync falling
#endif
	
	 NVIC_EnableIRQ(GPAB_IRQn);
	
}

void GPIO_Init(void)
{
//---------------------------------------------------------------------------------------------------------------------------------------
//------------------------------//	00 = INPUT mode / 01 = OUTPUT mode / 10 = Open-Drain mode / 11 = Quasi-bidirectional mode	
//	GPIO[PORT]		Value		//		15 14 13 12 / 11 10  9  8 /  7  6  5  4 /  3  2  1  0
//---------------------------------------------------------------------------------------------------------------------------------------
	GPIOA->u32PMD = 0X540A0005;	//	PA:	01 01 01 00 / 00 00 10 10 / 00 00 00 00 / 00 00 01 01
	GPIOB->u32PMD = 0X003D5A4A;	//	PB:	00 00 00 00 / 00 11 11 01 / 01 01 10 10 / 01 00 10 10
	GPIOC->u32PMD = 0X00415055;	//	PC:	00 00 -  -	/ 01 00 00 01 / 01 01 -  -	/ 01 01 01 01
//	GPIOD->u32PMD = 0X0000FFFF;	//	PD:	NA
	GPIOE->u32PMD = 0X00000000;	//	PE:				/			  /		  00	/	

	GPIOA->DOUT = 0XFFFD;	
	GPIOB->DOUT = 0XF73F;	
	GPIOC->DOUT = 0XFFFF;	
//	GPIOD->DOUT = 0XFFFF;	
	GPIOE->DOUT = 0X0000;	
}
void Fnd_Proc(void);

void TMR0_IRQHandler(void)
{
	TIMER0->TISR.TIF = 1;			// Clear timer0 interrupt flag

	tCnt_1ms++;
	Fnd_Cnt_1ms++;
	
	if(tCnt_1ms == 10)
	{
		tCnt_10ms++;
		tFlag_10ms = 1;
		tCnt_1ms = 0;
	}
	
	if(Fnd_Cnt_1ms == 3)
	{
		if(ctrl_connect_flag >= 2)
			Fnd_Proc();//Fnd_Proc();
		Fnd_Cnt_1ms = 0;
	}
	
	if(tCnt_10ms == 5)
	{
		tCnt_50ms++;
		tFlag_50ms = 1;
		tCnt_10ms = 0;
		if((stop_cnt_flag)&&(stop_cnt < STOP_CHK_TIME))		stop_cnt++;		//STOP status check timer
		if((reset_cnt_flag)&&(reset_cnt < RESET_CHK_TIME))	reset_cnt++;		//RESET status check timer
		if((move_cnt_flag)&&(move_cnt < MOVE_CHK_TIME))	move_cnt++;		//ERROR status check timer
	}
	if(tCnt_50ms == 2)
	{
		tFlag_100ms = 1;
		tCnt_100ms++;
		tCnt_50ms = 0;
	}
	if(tCnt_100ms == 2)
	{
		tCnt_100ms = 0;
		tCnt_200ms++;
		tFlag_200ms = 1;
	}
	if(tCnt_200ms == 5)
	{
		tCnt_200ms = 0;
		tFlag_1sec = 1;
	}
	
	if(tCnt_1sec == 3)
	{
		tCnt_1sec = 0;
		tFlag_3sec = 1;
	}
}

void TMR0_Init(void)
{
	// Step 1. Enable and Select Timer clock source
#if __USE_SYS_CLK__ == __XTAL
	SYSCLK->CLKSEL1.TMR0_S = 0;		// Select Timer0 clock source
#elif __USE_SYS_CLK__ == __IRC22M
	SYSCLK->CLKSEL1.TMR0_S = 7;		// Select Timer0 clock source
#endif
    SYSCLK->APBCLK.TMR0_EN = 1;		// Enable Timer0 clock source

	// Step 2. Select Operation mode
	TIMER0->TCSR.MODE = 1;			// Select operation mode

	// Step 3. Select Time out period = (Period of timer clock input) * (8-bit Prescale + 1) * (24-bit TCMP)
	TIMER0->TCSR.PRESCALE = 0;		// Set Prescale [0~255]
									// (1/22118400)*(0+1)*(2765)= 125.01usec or 7999.42Hz
	TIMER0->TCMPR  = __USE_SYS_CLK__/1000;	// Set TICR(TCMP) [0~16777215]  //10ms

	// Step 4. Enable interrupt
	TIMER0->TCSR.IE = 1;
	TIMER0->TISR.TIF = 1;			// Write 1 to clear for safty
	NVIC_EnableIRQ(TMR0_IRQn);		// Enable Timer0 Interrupt

	// Step 5. Enable Timer module
	TIMER0->TCSR.CRST = 1;			// Reset up counter
	TIMER0->TCSR.CEN = 1;			// Enable Timer0

//	TIMER0->TCSR.TDR_EN=1;			// Enable TDR function
}


void TMR1_IRQHandler(void)
{
	TIMER1->TISR.TIF |= 1;
	ENABLE_HINT;
}

void TMR1_Init(void)
{
	// Step 1. Enable and Select Timer clock source
#if __USE_SYS_CLK__ == __XTAL
	SYSCLK->CLKSEL1.TMR1_S = 0;		// Select Timer0 clock source
#elif __USE_SYS_CLK__ == __IRC22M
	SYSCLK->CLKSEL1.TMR1_S = 7;		// Select Timer0 clock source
#endif
    SYSCLK->APBCLK.TMR1_EN = 1;		// Enable Timer0 clock source

	// Step 2. Select Operation mode
//	TIMER1->TCSR.MODE = 0;			// Select operation mode
	setTMR1_ONESHOT;
	// Step 3. Select Time out period = (Period of timer clock input) * (8-bit Prescale + 1) * (24-bit TCMP)
//	TIMER1->TCSR.PRESCALE = 0;		// Set Prescale [0~255]
									// (1/22118400)*(0+1)*(2765)= 125.01usec or 7999.42Hz
	TIMER1->TCMPR  = H_Sync_CntDly;	// Set TICR(TCMP) [0~16777215] 

	// Step 4. Enable interrupt
//	TIMER1->TCSR.IE = 1;
//	TIMER1->TISR.TIF = 1;			// Write 1 to clear for safty
	NVIC_EnableIRQ(TMR1_IRQn);		// Enable Timer0 Interrupt

	// Step 5. Enable Timer module
	TIMER1->TCSR.CRST = 1;			// Reset up counter
//	TIMER1->TCSR.CEN = 1;			// Enable Timer0

//	TIMER1->TCSR.TDR_EN=1;			// Enable TDR function
}

void Send_Data(u08 ch, u32 tx_data)
{
	u08 txd[6];

	txd[0] = 0xa0;
	txd[1] = 0xff;
	txd[2] = ch;
	txd[3] = tx_data >> 8;
	txd[4] = tx_data;
	txd[5] = txd[0]+txd[1]+txd[2]+txd[3]+txd[4];
	
	uart_send(UART_PORT1, txd, 6);
}

// Debug Control -----------------------------------------------
#ifdef _DBG_PRINT_
void dbg_start(void)
{
	u32 rst_stat = 0;
	u32 rev = 0;
	
	UART_Init(DEBUG_PORT,BAUD115_2K);
	delay_ms(10);
	rev = (DrvGPIO_GetPortBits(E_GPC) & 0x0E00) >> 9;
	
	rst_stat = DrvSYS_GetResetSource();
	printf("\n\n%s Start![%02x -",_CODE_PRODUCT_,rst_stat);
	if(rst_stat&BIT7)	printf(" CPU");
	if(rst_stat&BIT5)	printf(" SYS");
	if(rst_stat&BIT4)	printf(" BOD");
	if(rst_stat&BIT3)	printf(" LVR");
	if(rst_stat&BIT2)	printf(" WDT");
	if(rst_stat&BIT1)	printf(" RST");
	if(rst_stat&BIT0)	printf(" POR");
	printf("]\n");
	switch(rev)
	{
		case 0:
			printf("SW ver:%s, release date:%s, %s\n",_CODE_VERSION_,_CODE_DATE_,_CODE_TIME_);
		break;
	}
	
	printf("\n");
}

void dbg_proc(u08 *rx_data)
{
	switch(rx_data[0])
	{
		case 'R':	case 'r':
			if(((rx_data[1] == 'S')||(rx_data[1] == 's'))
			&& ((rx_data[2] == 'T')||(rx_data[2] == 't')))	{SOH701_Reset();	System_Reset();}
		break;
		case 0x0d:							// enter
		case '>':
		break;
	}
}
#endif

void UartRxInt_Proc(UART_Ctrl_Rx *rx)
{
	#ifdef _DBG_PRINT_
//	printf("rx step: %2x \n", rx->step);
//	printf("rx->buff[0]: %2x \n", rx->buff[0]);
	#endif
	if(rx->step > 2)
		rx->step = 0;
	switch(rx->step)
	{
		case 0:
			if( rx->buff[0] == 0x1b )	rx->step = 1;							// Received ESC ?
			else if((SysMode == SERIAL_MENU_3) && (rx->buff[0] == 'y'))
			{
				Save_Flag = 1;
			}
			else if((Cursor_Pos_Row == 0) && (Cursor_Pos_Col == 0)){		// 리셋층 설정 위치 이면
				if( ((Cursor == 0) && (rx->buff[0] == 0x2d))					// 앞자리에서만 '-' 허용
					|| ((rx->buff[0]>0x2f) && (rx->buff[0]<0x3a)) )					// 0 ~ 9
				Save_Flag = 1;
//				printf("Save_Flag : %d \n", Save_Flag);
			}
			else if( (rx->buff[0] == 0x20) || (rx->buff[0] == 0x2d)					// ' ', '-'
					|| ((rx->buff[0]>0x2f) && (rx->buff[0]<0x3a))					// 0 ~ 9
					|| ((rx->buff[0]>0x40) && (rx->buff[0]<0x5b))					// A ~ Z
					|| ((rx->buff[0]>0x60) && (rx->buff[0]<0x7b)) ){				// a ~ z
				Save_Flag = 1;
			}
			rx1.recv_flag = 1;
			break;
		case 1:
			if( rx->buff[0] == '[' )	rx->step = 2;							// Received '[' ?
			else	rx->step = 0;
			break;
		case 2:
			if((rx->buff[0] == 'A') || (rx->buff[0] == 'B') || (rx->buff[0] == 'C') || (rx->buff[0] == 'D')){		// Arrow keys ?
				Arrow_Flag = 1;
			}
			rx->step = 0;
			rx1.recv_flag = 2;
			break;
	}
//	printf("------------------------\n");
//	printf("rx->step: %02x \n", rx->step);
//	printf("rx->buff[0]: %02x \n", rx->buff[0]);
}

// Uart Control -----------------------------------------------
void Uart_Proc(u08 *rx_data, u08 rx_flag)
{
	switch(rx_flag)
	{
		case 1:		// 
			#ifdef _DBG_PRINT_
			printf("rx : %02x %02x %02x %02x %02x %02x\n", rx_data[0], rx_data[1], rx_data[2], rx_data[3], rx_data[4], rx_data[5]);
			#endif
			break;
		case 2:		// 
			break;
		#ifdef _DBG_PRINT_
		case 3:	dbg_proc(rx_data); break;
		#endif
	}
}

// Laser Zig Control -----------------------------------------------
void ZigOff_Check(void)
{
	if(zig_sw_flag){
		zig_on_count++;
		if(zig_on_count == LASER_ON_TIME){
			LASER_ZIG_OFF;
			zig_sw_flag = 0;
			zig_on_count = 0;
		}
	}
}

// UTC Control -----------------------------------------------
void Utc_Video_Check(void)
{
	u08 video_temp = 0;
	video_signal = DrvGPIO_GetBit(E_GPC, 14) << 1;
	video_signal |= DrvGPIO_GetBit(E_GPC, 15);

	video_temp = DrvGPIO_GetBit(E_GPB, VID_FMT2) << 2;
	video_temp |= DrvGPIO_GetBit(E_GPB, VID_FMT1) << 1;
	video_temp |= DrvGPIO_GetBit(E_GPB, VID_FMT0) << 0;
	
	
	if(video_signal != pre_video_signal)
	{	
		//printf("UTC video check RESET\n");
		SOH701_Reset();
		System_Reset();
	}
	
	pre_video_signal = video_signal;
/*
	#ifdef _DBG_PRINT_
	switch(video_signal)
	{
		case 0: //CVBS
			video_signal = CVBS;
			printf("UTC: CVBS \n");
		break;
		case 1: //TVI
			video_signal = TVI;
			printf("UTC: TVI \n");
		break;
		case 2: //CVI
			video_signal = CVI;
			printf("UTC: CVI \n");
		break;
		case 3: //AHD
			video_signal = AHD;
			printf("UTC: AHD \n");
		break;
	}

	switch(video_temp)
	{
		case RESOLUTION_PAL:
			printf("Video Format : PAL \n");
		break;
		case RESOLUTION_NTSC:
			printf("Video Format : NTSC \n");
		break;
		case RESOLUTION_HD25P:
			printf("Video Format : HD 25P \n");
		break;
		case RESOLUTION_HD30P:
			printf("Video Format : HD 30P \n");
		break;
		case RESOLUTION_FULLHD25P:
			printf("Video Format : Full HD 25P \n");
		break;
		case RESOLUTION_FULLHD30P:
			printf("Video Format : Full HD 30P \n");
		break;
		case RESOLUTION_3MP:
			printf("Video Format : 3 Mega Pixel \n");
		break;
		case RESOLUTION_4MP:
			printf("Video Format : 4 Mega Pixel \n");
		break;
	}
#endif
*/
}

void TmrCntSrv_Proc(void)
{
	Vsync_Chk_Proc();
	if(V_Sync_Stat == 0)
	{
		if(GET_BIT(UTC_EN_PIN, UCC_Data_EN) == 0)	UTC_OFF;
	}
#ifdef __USE_WDT__
	WdtRstFlag = 1;
#endif
	
}

void Set_Sync_Int(u08 h_set, u08 v_set)
{
	if(h_set == 0){			DISABLE_HINT;	}
	else if(h_set == 1){	ENABLE_HINT;	}	

#if _USE_ON_OFF_VINT_
	if(v_set == 0){			DISABLE_VINT;	}
	else if(v_set == 1){	ENABLE_VINT;	}
#endif

}

void SOH701_Reset(void)
{
#ifdef _DBG_PRINT_
	printf("RESET!! \n");
#endif

	DrvGPIO_ClrBit(E_GPC, BD_REV3);
	delay_ms(10);
	DrvGPIO_SetBit(E_GPC, BD_REV3);
}
u08 tdr_flag = 0;
void Video_Resolution_Check(void)
{
	//clk = 12M
	if((TDR_Buff > 254) && (TDR_Buff < 281))				video_format = HD_720P60;
	else if((TDR_Buff > 298) && (TDR_Buff < 325))			video_format = HD_720P50;
	else if((TDR_Buff > 341) && (TDR_Buff < 368))			video_format = HD_1080P30;
	else if((TDR_Buff > 406) && (TDR_Buff < 433))			video_format = HD_1080P25;
	else if((TDR_Buff > 514) && (TDR_Buff < 542))			video_format = HD_720P30;
	else if((TDR_Buff > 623) && (TDR_Buff < 650))			video_format = HD_720P25;
	// CVBS 
	else if(((pre_video_temp == 0) || (pre_video_temp == SD_NTSC)) && (TDR_Buff > 737) && (TDR_Buff < 763))	// NTSC max 768		
		video_format = SD_NTSC;
	else if(((pre_video_temp == 0) || (pre_video_temp == SD_PAL)) && (TDR_Buff > 766) && (TDR_Buff < 770))	// PAL min 763
		video_format = SD_PAL;
	
	/*	in case of "clk = internal 22M"
	if((TDR_Buff > 470) && (TDR_Buff < 520))				video_format = HD_720P60;
	else if((TDR_Buff > 550) && (TDR_Buff < 600))			video_format = HD_720P50;
	else if((TDR_Buff > 630) && (TDR_Buff < 680))			video_format = HD_1080P30;
	else if((TDR_Buff > 750) && (TDR_Buff < 800))			video_format = HD_1080P25;
	else if((TDR_Buff > 950) && (TDR_Buff < 1000))			video_format = HD_720P30;
	else if((TDR_Buff > 1150) && (TDR_Buff < 1200))			video_format = HD_720P25;
	else if((TDR_Buff > 1360) && (TDR_Buff < 1410))			video_format = SD_NTSC;
	else if((TDR_Buff > 1410) && (TDR_Buff < 1420))			video_format = SD_PAL;
	*/
//	printf("TDR_Buff : %d \n", TDR_Buff);
	
	if((pre_video_temp != video_format) && (pre_video_temp > 2))
	{
		SOH701_Reset();
		System_Reset();
	}
	
	if(TDR_Buff || DrvGPIO_GetBit(E_GPB, V_LOSS))	// when device boot, osd auto out(if device reset with osd)
	{
		osd_boot_cnt++;
		osd_init_cnt++;
		if(osd_boot_cnt == 25)
			osd_boot_flag = 1;
		if(osd_out_flag == 3)
		{
			if(osd_init_cnt == 25)
			{
				Osd_Erase(row_data);		// 9/12 modi
				delay_ms(1);				// 9/12 modi
				osd_out_flag = 1;
			}
		}		
	}
	tdr_flag++;
	pre_video_temp = video_format;
	if(tdr_flag == 25)
	{
		Utc_Video_Check();	
//		printf("TDR_Buff:%d\n", TDR_Buff);
//		printf("pre_video_temp : %d\n", pre_video_temp);
//		printf("video_format : %d\n", video_format);
		tdr_flag = 0;
	}
}

void Hsync_Chk_Proc(void)
{
	// dܳ{N V-Sync Ǟݺ T؂N¶ üũ
//	if((H_Sync_Cnt > 0) && (H_Sync_Cnt < 3)){	if(DrvGPIO_GetBit(E_GPA, V_SYNC)){ printf("Vsync hi! ");	} }//V_Sync_Cnt = 0;} }

	// H ī࠮ƮС üũߡ Ȋࠤȑ ݶئ ԑ8٩ H-Int. OFF, V-Int. ON
	
	if(V_Sync_Stat >= 1)
	{
#if _USE_H_TIMING_CHK_ == 1
		if(H_Sync_Cnt > CHK_END_HSYNC_CNT)
#else
		if(H_Sync_Cnt > Utc_Off_Cnt)
#endif
		{
			Set_Sync_Int(OFF, ON);	// set int h:off, s:on
			H_Sync_Cnt = 0;
#if _USE_H_TIMING_CHK_ != 2
			H_Sync_ChkDly = 2;
#endif
		}
	}
	else
	{
		if(H_Sync_Cnt >= NOVID_OFF_HSYNC_CNT)
		{
			Set_Sync_Int(OFF, ON);	// set int h:off, s:on
			H_Sync_Cnt = 0;
		}
	}

#if _USE_H_TIMING_CHK_
	// H-Period(Șܳյ)С گѦևߺՂ¶ üũ
	if(H_Sync_ChkDly == 2)
	{
		H_Sync_ChkDly = 0;

		TDR_Buff /= 10;
		
#if _USE_H_TIMING_CHK_ == 2
		H_Sync_CntDly = tdr_buf;

#endif

		V_Sync_Stat = 2;
	}
#endif
}

void Vsync_Chk_Proc()
{
	// V-Sync LossС և٩ RESET TIME իҢ Ʉ ՙރ V int üũ ރ[
	if(V_Sync_Err)
	{
#if _MODE_SET_FUNC_
		if(UtcEn_Start)
#endif
		{
			//#ifdef _DBG_PRINT_
			//printf(" V_Sync_ErrCnt = %d\n", V_Sync_ErrCnt);
			//#endif

			if(V_Sync_ErrCnt == 4)
			{
				V_Sync_ErrCnt = 0;
				V_Sync_Err = 0;
				V_Sync_Cnt = 0;
				H_Sync_Cnt = 0;
				H_Sync_ChkDly = 0;
	#if _USE_ON_OFF_VINT_
				Set_Sync_Int(2, ON);	// set int h:ignore, s:on
				#ifdef _DBG_PRINT_
//				printf("V_Sync check start (GPA_IEN = %08lx)\n",GPIOA->IEN);
				#endif
	#endif
	#if _USE_H_TIMING_CHK_ == 1
				 TMR1_READY_ONESHOT; 
	#endif
			}
			else	V_Sync_ErrCnt++;
		}
	}
	else
	{
		
		if((V_Sync_Cnt > 0) && (V_Sync_Cnt < 35))
		{
			if(V_Sync_Stat == 0)
			{
				#ifdef _DBG_PRINT_
				printf("0_V_Sync_Cnt = %d\n", V_Sync_Cnt);
				#endif
				if(V_Sync_Cnt > 3){
					V_Sync_Stat = 1;
					#ifdef _DBG_PRINT_
					printf("sync in(%d)\n", V_Sync_Stat);
					#endif
					#if _USE_H_TIMING_CHK_ == 2
					Send_Data(ch, 'G');
					#endif
				}
			}
/*			else
			{
				#ifdef _DBG_PRINT_
				if(abs_diff(V_Sync_Cnt_Buff, V_Sync_Cnt) > 1){
					V_Sync_Cnt_Buff = V_Sync_Cnt;
					printf("1_V_Sync_Cnt = %d\n", V_Sync_Cnt);
				}
				#endif
			}
*/			V_Sync_ErrCnt = 0;
		}
		else
		{
			if(V_Sync_Stat >= 1)
			{
				// V-Sync LossС և٩ UTC EN, V,H int off
				if(V_Sync_ErrCnt == 3)
				{
#if _USE_ON_OFF_VINT_
					Set_Sync_Int(OFF, OFF);		// set int h:off, s:off
#else
					Set_Sync_Int(OFF, 2);		// set int h:off, s:ignore
#endif	
					// UTC_EN off
					UTC_OFF; 	
					UtcEn_Start = 0;
					V_Sync_Stat = 0;
					V_Sync_ErrCnt = 0;
					V_Sync_Err = 1;
					V_Sync_Cnt = 0;
					H_Sync_Cnt = 0;
					H_Sync_ChkDly = 0;
					SOH701_Reset();
					//printf("Vsync_Chk_Proc RESET\n");
					//System_Reset();
					#ifdef _DBG_PRINT_
					printf("sync loss(%d)(GPIOA->IEN = %08x)\n", V_Sync_Stat, GPIOA->IEN);
					#endif
				}
				else	V_Sync_ErrCnt++;
			}
		}

		V_Sync_Cnt = 0;
	}	
}
void UTC_Mode_Set(u08 mode)
{
	// UTC_EN off
	 UTC_OFF;

	UtcEn_Start = 0;
	V_Sync_Stat = 0;
	V_Sync_ErrCnt = 0;
	V_Sync_Err = 1;
	V_Sync_Cnt = 0;
	H_Sync_Cnt = 0;
	H_Sync_ChkDly = 0;

	if(mode == 0x01){
		Utc_On_Cnt  = TVI_ON_HSYNC_CNT;
		Utc_Off_Cnt = TVI_OFF_HSYNC_CNT;
	}
	else if(mode == 0x03){
		Utc_On_Cnt  = AHD_ON_HSYNC_CNT;
		Utc_Off_Cnt = AHD_OFF_HSYNC_CNT;
	}
	else if(mode == 0x02){
		Utc_On_Cnt  = CVI_ON_HSYNC_CNT;
		Utc_Off_Cnt = CVI_OFF_HSYNC_CNT;
	}
	else
	{
		Utc_On_Cnt  = CVBS_ON_HSYNC_CNT;
		Utc_Off_Cnt = CVBS_OFF_HSYNC_CNT;
	}

	H_Sync_CntDly = INT_ON_HSYNC_DLY;
//	V_Sync_Err = 1;
	
#if _USE_ON_OFF_VINT_
	Set_Sync_Int(OFF, OFF);	// set int h:off, s:off
#else
	Set_Sync_Int(OFF, 2);
#endif	
}

void UTC_Init(void)
{
	pre_video_signal = DrvGPIO_GetBit(E_GPC, 14) << 1;
	pre_video_signal |= DrvGPIO_GetBit(E_GPC, 15);
	UTC_Mode_Set(pre_video_signal);
	GPIO_INT_Init();
	Utc_Video_Check();
	TMR1_Init();
}

void Loss_Change_Chk(void)
{
	u08 v_loss;
	v_loss = DrvGPIO_GetBit(E_GPB, V_LOSS);
	if(v_loss)
	{
		printf("v_loss\n");
		Osd_Clear();
		delay_ms(3);
		SOH701_Reset();
//		System_Reset();
	}
}

// Key Check -----------------------------------------------
 u08 Key_Check(void)
{
	u08 i, key_temp = 0;
	u08 key_read_buff = 0;
	u08 key_val;
	
	key_read_buff = (~DrvGPIO_GetPortBits(E_GPA) & 0x38) >> 3;
	key_read_buff |= (~DrvGPIO_GetBit(E_GPA, CTRLER_RESET)) << 7 ;
	key_read_buff |= (~DrvGPIO_GetPortBits(E_GPC) & 0x600) >> 6;
	key_read_buff |= (~DrvGPIO_GetPortBits(E_GPC) & 0xC0) >> 1;	//key_read_buff = MODE / SW2 / SW1 / UP / LEFT / DOWN / RIGHT / SET  
	key_temp = key_read_buff;
	
	if(key_temp != 0)											// if key input
	{
		if(key_comp == key_temp)
		{
			key_rechk_cnt--;
			if(key_rechk_cnt == 0)
			{				
				if(key_comp != 0)						
				{
//s					printf("key_temp : %x \n", key_temp);
					if(!key_rechk_flag)
					{
						if(key_read_buff == 0x60)
						{
							key_rechk_flag = 1;
						}	
						else
						{
							for(i=0; i<8; i++)
							{
								if(key_read_buff & (0x01 << i))
									break;
							}
							
							key_val = 0x01 << i;
							key_rechk_flag = 1;
						}
					}
					else if(key_rechk_flag == 1 || key_rechk_flag == 3)
					{
						if(key_read_buff == 0x60)
						{
							key_rechk_flag = 2;
							key_val = 0x60;
						}
						else
						{
							for(i=0; i<8; i++)
							{
								if(key_read_buff & (0x01 << i))
									break;
							}
							key_val = 0x01 << i;
							key_rechk_flag = 2;
						}
					}
				}
				key_rechk_cnt = KEY_REPEAT_TIME;	
			}
		}
		else
		{
			key_comp = key_temp;
			if(key_temp != 0x60)
				key_rechk_cnt = KEY_STABLE_TIME;
		}
	}
	else
	{
		key_comp = 0;
		key_rechk_cnt = KEY_STABLE_TIME;
		key_rechk_flag = 0;
	}
	
	return key_val;	
}
void CFG_Save_Sync_Off(u08 page)
{
	Set_Sync_Int(0,0);
	CFG_Save(page);
	Set_Sync_Int(0,1);
}
void Key_Proc(void)
{
	u08 key_val = 0;
	key_val = Key_Check();
	

	switch(key_val)
	{
		case KEY_SET:
		break;
		case KEY_UP:
		break;
		case KEY_DOWN:
		break;
		case KEY_RIGHT:
		break;
		case KEY_LEFT:
		break;
#ifdef TEST_MODE
		case (KEY_LASER + KEY_OSD):
			if(key_rechk_flag == 2)
			{
				test_mode = 1;
				LASER_ZIG_OFF;
				Osd_Clear();
				delay_ms(3);
				osd_position = 0;
				Floor.current = 11;
				Display_TEST();
				tCnt_200ms = 0;
			}						
		break;
#endif
		case KEY_LASER:
			if(key_rechk_flag == 1)
			{
				if(zig_sw_flag)
				{
					LASER_ZIG_OFF;
					zig_sw_flag = 0;
				}
				else
				{
					LASER_ZIG_ON;
					zig_sw_flag = 1;
				}
			}
			else if(key_rechk_flag == 2)
			{
				if(rule_flag != RULE_MOVE)
				{
					if(door_step == 2)
						door_step = 3;
					else if(door_step == 3)
						door_step = 0;
					Osd_Erase(5);
					Osd_Locate(2, 2);
					if(!door_step)
						Osd_PutStr(PAGE_ENGLISH, (u08*)"Door Open Check");
					else if(door_step == 1)
						Osd_PutStr(PAGE_ENGLISH, (u08*)"Door Close Check");
					
					door_flag = 1;
					key_rechk_flag =3;
				}
			}
			
		break;
		case KEY_OSD:
	/*		
			if(!osd_flag)
			{
				test_FloorDisplay();
				osd_flag = 1;
			}
			else
			{
				Osd_Erase(row_data);
				osd_flag = 0;
			}
	*/
			if(key_rechk_flag == 1)
			{
				osd_out_flag = 2;
				Floor_Display(osd_position);
				delay_ms(2);
				osd_position++;
				if(osd_position == 7)
					osd_position = 0;
				else if(osd_position)
					osd_out_flag = 1;
//				Rs485_Send(1);
			#ifdef _DBG_PRINT_
				printf("osd_pos : %d cur : cur_floor : %d %d \n", osd_position, osd_floor[Floor.current][0], osd_floor[Floor.current][1]);
			#endif
			}
			else if(key_rechk_flag == 2)
				id_flag = 1;
									
		break;
		case KEY_MODE:
		break;
		
		
	}
	
	if(ctrl_connect_flag >= 2)
		Contoller_Key_Proc(key_val);	
}

void ID_Off_check(void)
{
	if(id_flag == 2)
	{
		key_rechk_flag_cnt++;
		if(key_rechk_flag_cnt == 50)
		{
			Display_ID();
			key_rechk_flag_cnt = 0;
		}
	}
}
// OSD Check -----------------------------------------------
void Move_Stop(void)
{
	move_step = 0;
	move_stat_chk_flag = 0;
	rule_flag = RULE_STOP;
	move_flag = MOVE_NONE;
	stop_cnt_flag = 0;
	stop_cnt = 0;
	reset_cnt_flag = 0;
	reset_cnt = 0;
}
void PhotoSensor_Check(void)
{
#ifndef _720p_use	
	if((!video_format) || (video_format < 3) || (video_format > 6))			
#endif
	switch(rule_flag)
	{
		case RULE_NONE:
			if(((pre_sensor&0x01) == 0x01) && ((cur_sensor&0x01) == 0x01))
				rule_flag = RULE_STOP;
		break;
			
		case RULE_STOP:
			
			if(!move_step)
			{
				if((pre_sensor == 0x03) && (cur_sensor == 0x01))								//Base Floor -> other Floor (down)
				{	
					move_flag = MOVE_ING_DOWN;
					osd_out_flag = 1;
				#ifdef _DBG_PRINT_
					printf("MOVE_ING_DOWN move_flag(switch) : %d \n", move_flag);
				#endif
				}
				else if((pre_sensor == 0x03) && (cur_sensor == 0x02))						//Base Floor -> other Floor (up)
				{	
					move_flag = MOVE_ING_UP;
					osd_out_flag = 1;
				#ifdef _DBG_PRINT_
					printf("MOVE_ING_UP move_flag : %d \n", move_flag);
				#endif
				}
				else if((pre_sensor == 0x01) && (cur_sensor == 0x00))						//a Floor -> other Floor 
				{
					move_step = 1;
					move_stat_chk_flag = 1;
				}
			}
			else
			{
				if((pre_sensor == 0x00) && (cur_sensor == 0x02))
				{
					move_flag = MOVE_ING_UP;	
					rule_flag = RULE_MOVE;
					osd_out_flag = 1;
				}
			}
						
			if((pre_sensor == 0x00) && (cur_sensor == 0x00))	// RULE: STOP -> MOVE
			{
				if(move_cnt_flag == 0)	move_cnt_flag = 1;			
				if(move_cnt == MOVE_CHK_TIME)						
				{
					move_cnt_flag = 0;
					move_cnt = 0;
					rule_flag = RULE_MOVE;
				#ifdef _DBG_PRINT_
					printf("RULE_MOVE \n");
				#endif
				}
			}
			else if(cur_sensor == 0x01)		// prevent error
			{
				move_flag = MOVE_NONE;	
				rule_flag = RULE_STOP;
				move_cnt_flag = 0;
				move_cnt = 0;
			}
		break;
			
		case RULE_MOVE:
			cfg_flag = 0;
			if(pre_sensor == 0x00)
			{
//				printf("RULE_MOVE move_flag : %d \n", move_flag);
//				printf("move_step : %d \n", move_step);
//				printf("cur_sensor : %d \n", cur_sensor);
					
				switch(move_flag)
				{
					case MOVE_NONE:
						if(move_stat_chk_flag)
						{
							move_flag = MOVE_ING_DOWN;
							osd_out_flag = 1;
							move_stat_chk_flag = 0;
						}
					break;
						
					case MOVE_ING_UP:
						if(cur_sensor == 0x01)
						{
							Set_Sync_Int(0,0);
							UTC_OFF;
							
							if(Floor.current < 109)
							{
								Floor.current++;
								while(((osd_floor[Floor.current][0] == 'X') 
								&& ((osd_floor[Floor.current][1] == ' ') || (osd_floor[Floor.current][1] == 'X'))) 
								|| (Floor.current == 10))
									Floor.current++;
								osd_out_flag = 1;
							}
							
							
							Set_Sync_Int(0,1);
						#ifdef _DBG_PRINT_
							printf("Floor count + 1 // cur_floor : %d \n", Floor.current);
						#endif
						}
					break;
					
					case MOVE_ING_DOWN:
						if(cur_sensor == 0x02)
						{
							Set_Sync_Int(0,0);
							UTC_OFF;
							
							
							if(Floor.current > 1)
							{
								Floor.current--;
																	
								while(((osd_floor[Floor.current][0] == 'X') && ((osd_floor[Floor.current][1] == ' ') || (osd_floor[Floor.current][1] == 'X'))))
								{
									Floor.current--;
								}
							
								osd_out_flag = 1;
							}	

							Set_Sync_Int(0,1);
						#ifdef _DBG_PRINT_
							printf("Floor count - 1 // cur_floor : %d \n", Floor.current);
						#endif
						}
					break;
				}
			}
	}

	if((rule_flag == RULE_STOP) && (pre_sensor == 0x03) && (cur_sensor == 0x03))
	{
		if(reset_cnt_flag == 0)	reset_cnt_flag = 1;						// Reset rule check start
		if(reset_cnt == RESET_CHK_TIME)								// Reset?
		{
			Floor_Reset_Display();
			
			if(cfg_flag == 0 || cfg_flag == 1)
			{
				CFG_Save_Sync_Off(PAGE1);
				cfg_flag = 2;
			}
						
			Move_Stop();
						
		#ifdef _DBG_PRINT_
			printf("RESET FLOOR!\n");
		#endif
		}
	}
	else if(((pre_sensor&0x01) == 0x01) && ((cur_sensor&0x01) == 0x01))			// Top Sensor Detect continue
	{
		if(stop_cnt_flag == 0)	stop_cnt_flag = 1;						// Move Stop rule check start
		if(stop_cnt == STOP_CHK_TIME)									// Move Stop?
		{ 
			if(!cfg_flag)
			{
				CFG_Save_Sync_Off(PAGE1);
				cfg_flag = 1;
			}
			
			osd_out_flag = 1;
			
			Move_Stop();
			
		#ifdef _DBG_PRINT_
			printf("RULE_STOP\n");
		#endif
		}
	}
	else
	{
		reset_cnt_flag = 0;
		reset_cnt = 0;
		stop_cnt_flag = 0;
		stop_cnt = 0;
	}
	
	pre_sensor = cur_sensor;
}

// Floor Control -----------------------------------------------
void test_FloorDisplay(void)
{
	u08 osd_char_temp[1];
	u16 i, j;
	
	switch(video_format)
	{
		case SD_PAL:
		case SD_NTSC:
			row_data = 12;
			for(i=1; i<=12; i++)
			{
				for(j=1; j<=24; j++)
				{
					Osd_Locate(i, j);
					osd_char_temp[0] = i%13 + (j%24-1) + 0x30;//j%23;
					Osd_PutChar(PAGE_ENGLISH, osd_char_temp, 1);
				}
			}
			break;
#ifdef _720p_use
		case HD_720P25:
		case HD_720P30:
		case HD_720P50:
		case HD_720P60:
			row_data = 16;
			for(i=1; i<=16; i++)
			{
				for(j=1; j<=24; j++)
				{
					Osd_Locate(i, j);
					osd_char_temp[0] = i%17 + (j%24-1) + 0x30;//j%23;
					Osd_PutChar(PAGE_ENGLISH, osd_char_temp, 1);
				}
			}
		break;
#endif
		case HD_1080P25:
		case HD_1080P30:
			row_data = 15;
			for(i=1; i<=15; i++)
			{
				for(j=1; j<=24; j++)
				{
					Osd_Locate(i, j);
					osd_char_temp[0] = i%16 + (j%24-1) + 0x30;//j%23;
					Osd_PutChar(PAGE_ENGLISH, osd_char_temp, 1);
				}
			};
		break;
		case UHD_3MP:
			
		break;
		case UHD_4MP:
			
		break;
	}
}

void Osd_Pos_Sel(u08 pos)
{
	/*
	switch(video_signal)
	{
		case 0: //CVBS
			printf("video_signal: CVBS \n");
		break;
		case 1: //TVI
			printf("video_signal: TVI \n");
		break;
		case 2: //CVI
			printf("video_signal: CVI \n");
		break;
		case 3: //AHD
			printf("video_signal: AHD \n");
		break;
	}
	
	switch(video_format)
	{
		case SD_PAL: 
			printf("video_format: SD_PAL \n");
		break;
		case SD_NTSC: 
			printf("video_format: SD_NTSC \n");
		break;
		case HD_720P25: ;
			printf("video_format: HD_720P25 \n");
		break;
		case HD_720P30: 
			printf("video_format: HD_720P30 \n");
		break;
		case HD_1080P25: 
			printf("video_format: HD_1080P25 \n");
		break;
		case HD_1080P30: 
			printf("video_format: HD_1080P30 \n");
		break;
	}
	*/
//	printf("video_format : %d \n", video_format);
	switch(video_format)
	{
		case SD_PAL:
			switch(pos)
			{
				case 1: Osd_Locate(1, 2); break;
				case 2: Osd_Locate(1,11); break;
				case 3: Osd_Locate(1,21); break;
				case 4: Osd_Locate(11, 2); break;
				case 5: Osd_Locate(11,11); break;
				case 6: Osd_Locate(11,21); break;
			}
		break;
		case SD_NTSC:
			row_data = 12;
			switch(pos)
			{
				case 1: Osd_Locate(1, 2); break;
				case 2: Osd_Locate(1,11); break;
				case 3: Osd_Locate(1,21); break;
				case 4: Osd_Locate(12, 2); break;
				case 5: Osd_Locate(12,11); break;
				case 6: Osd_Locate(12,21); break;
			}
		break;
#ifdef _720p_use
		case HD_720P25:	case HD_720P30:	case HD_720P50:	case HD_720P60:
			row_data = 16;
			if(video_signal == TVI)
			{
//				if(video_format == HD_720P30)
					switch(pos)
					{
						case 1: Osd_Locate(1, 1); break;
						case 2: Osd_Locate(1, 6); break;
						case 3: Osd_Locate(1, 11); break;
						case 4: Osd_Locate(16, 1); break;
						case 5: Osd_Locate(16, 6); break;
						case 6: Osd_Locate(16, 11); break;
					}
			}
			else if(video_signal == CVI)
			{
				if(video_format == HD_720P30)
					switch(pos)
					{
						case 1: Osd_Locate(1, 1); break;
						case 2: Osd_Locate(1,10); break;
						case 3: Osd_Locate(1,21); break;
						case 4: Osd_Locate(16, 1); break;
						case 5: Osd_Locate(16,10); break;
						case 6: Osd_Locate(16,21); break;
					}
				else
					switch(pos)
					{
						case 1: Osd_Locate(1, 1); break;
						case 2: Osd_Locate(1, 16); break;
						case 3: Osd_Locate(16, 1); break;
						case 4: Osd_Locate(16, 16);	break;
						case 5: 
						case 6: osd_position = 0; osd_out_flag = 2;
						break;
					}
				
			}
			else if(video_signal == AHD)
			{
				switch(pos)
				{
					case 1: Osd_Locate(1, 1); break;
					case 2: Osd_Locate(1,10); break;
					case 3: Osd_Locate(1,20); break;
					case 4: Osd_Locate(16, 1); break;
					case 5: Osd_Locate(16,10); break;
					case 6: Osd_Locate(16,20); break;
				}
			}
		break;
#endif	
		case HD_1080P25:	case HD_1080P30:
			row_data = 15;
			if(video_signal == TVI)
			{
//				if(video_format == HD_1080P30)
					switch(pos)
					{
						case 1: Osd_Locate(1, 1); break;
						case 2: Osd_Locate(1,11); break;
						case 3: Osd_Locate(1,20); break;
						case 4: Osd_Locate(15, 1); break;
						case 5: Osd_Locate(15,11); break;
						case 6: Osd_Locate(15,20); break;
					}
			}
			else if(video_signal == CVI)
			{
				if(video_format == HD_1080P30)
					switch(pos)
					{
						case 1: Osd_Locate(1, 2); break;
						case 2: Osd_Locate(1, 11); break;
						case 3: Osd_Locate(1, 21); break;
						case 4: Osd_Locate(15, 2); break;
						case 5: Osd_Locate(15,11); break;
						case 6: Osd_Locate(15,21); break;
					}
				else
					switch(pos)
					{
						case 1: Osd_Locate(1, 5); break;
						case 2: Osd_Locate(1, 13); break;
						case 3: Osd_Locate(1, 21); break;
						case 4: Osd_Locate(15, 5); break;
						case 5: Osd_Locate(15,13); break;
						case 6: Osd_Locate(15,21); break;
					}
			}
			else if(video_signal == AHD)
			{
				switch(pos)
				{
					case 1: Osd_Locate(1, 1); break;
					case 2: Osd_Locate(1,11); break;
					case 3: Osd_Locate(1,21); break;
					case 4: Osd_Locate(15, 1); break;
					case 5: Osd_Locate(15,11); break;
					case 6: Osd_Locate(15,21); break;
				}
			}
		break;
		case UHD_3MP:
			row_data = 16;
		break;
		case UHD_4MP:
			
		break;
	}
	
}

void Display_ID(void)
{
	u08 id_temp[3] = {0,};
	u08 id_erase[6] = {' ', ' ', ' ', ' ', ' ', ' '  };
	
	if(id_flag)
	{
		delay_ms(2);
		switch(video_format)
		{
			case SD_PAL:	case SD_NTSC:
				Osd_Locate(6,10);
			break;	
#ifdef _720p_use		
			case HD_720P25:	case HD_720P30:	case HD_720P50:	case HD_720P60:
				
				if(video_signal == TVI)
					Osd_Locate(8,5);
				else if(video_signal == CVI)
				{
					if(video_format == HD_720P30)
						Osd_Locate(8,9);	
					else
						Osd_Locate(8, 15);
				}
				else if(video_signal == AHD)
					Osd_Locate(8,9);
			break;
#endif
			case HD_1080P25:	case HD_1080P30:
				
				if(video_signal == TVI)
					Osd_Locate(8,10);
				else if(video_signal == CVI)
				{
					if(video_format == HD_1080P30)
						Osd_Locate(8,10);	
					else
						Osd_Locate(8,12);
				}
				else if(video_signal == AHD)
					Osd_Locate(8,10);
			break;
			case UHD_3MP:
				
			break;
			case UHD_4MP:
				
			break;
		}
		
		delay_ms(1);
//		printf("id_flag : %d \n", id_flag);
		
		if(id_flag == 1)
		{
			Osd_PutStr(PAGE_ENGLISH, (u08*)"ID:");
			delay_ms(1);
			id_temp[0] = rs485_id[2] + 0x30;
			id_temp[1] = rs485_id[1] + 0x30;
			id_temp[2] = rs485_id[0] + 0x30;
			Osd_PutChar(PAGE_ENGLISH, id_temp, 3);
			delay_ms(1);
			id_flag = 2;
		}
		else
		{
			Osd_PutChar(PAGE_ENGLISH, id_erase, 6);
			id_flag = 0;
		}
		
		key_rechk_flag = 3;		
	}	
}

u32 Door_Data_Comp(u32 a, u32 b, u08 c)
{
	if(c)
	{
		if(a>b)
			return a;
		else
			return b;
	}
	else
	{
		if(a<b)
			return a;
		else
			return b;
	}
}

u08 Door_Chk(void)
{
	u08 door_char = 0;
//	static u08 pre_door_char = 0;
	u32 door_temp = 0;
#ifdef PROX_SENSOR_USE
	u08 prox_sensor = 0;
	prox_sensor = DrvGPIO_GetBit(E_GPA, 6);
//	prox_sensor |= (DrvGPIO_GetBit(E_GPA, 7) << 1);

	if(prox_sensor)
		door_char = 0x1F;
	else
		door_char = 0x1E;

#else
	door_temp = Get_ADC(UP);
	if(door_temp < (door_max + door_min)/2)
		door_char = 0x1E;
	else
		door_char = 0x1F;

#endif
//	if((pre_door_char != door_char) && osd_out_flag != 3)
	if(osd_out_flag != 3)	
		osd_out_flag = 1;
//	pre_door_char = door_char;
	
	return door_char;
}

void Door_Data_Check(void)
{
	u32 door_temp = 0;
	static u32 door_comp = 0;
	
	if(rule_flag != RULE_MOVE)
	{
		switch(door_step)
		{
			case 0:		// close door data 
				if(door_flag)
				{
					door_cnt++;
					door_temp = Get_ADC(UP);
					if(!door_comp)
						door_comp = door_temp;
					else
						door_comp = Door_Data_Comp(door_comp, door_temp, 1);
					
					if(door_cnt == 20)
					{
						Osd_Locate(4, 2);
						Osd_PutStr(PAGE_ENGLISH, (u08*)"Complete");
						door_max = door_comp;
//						printf("door_max : %d \n", door_max);
						door_cnt = 0;
						door_comp = 0;
						door_step = 1;
						door_flag = 0;
					}
				}
			break;
			case 1:		// open door data
				if(door_flag)
				{
					door_cnt++;
					door_temp = Get_ADC(UP);
					if(!door_comp)
						door_comp = door_temp;
					else
						door_comp = Door_Data_Comp(door_comp, door_temp, 0);

					if(door_cnt == 20)
					{
						Osd_Locate(4, 2);
						Osd_PutStr(PAGE_ENGLISH, (u08*)"Complete");
						door_min = door_comp;
//						printf("door_min : %d \n", door_min);
						door_comp = 0;
						door_cnt = 0;
						osd_out_flag = 1;
						door_step = 2;
						CFG_Save_Sync_Off(PAGE1);
						Osd_Erase(row_data);
						delay_ms(2);
						osd_out_flag = 1;
					}
				}
				break;
			case 2:
				door_data = Door_Chk();
			break;
			case 3:
				door_data = ' ';
				door_max = 0;
				door_min = 0;
				osd_out_flag = 1;
			break;
		}
	}
	
	switch(door_data)
	{
		case 0x1E:
			door_rs_data = 1;
			break;
		case 0x1F:
			door_rs_data = 2;
			break;
		default : 
			door_rs_data = 0;
		break;
	}
}

void Floor_Display(u08 pos)
{
	u08 osd_char_temp[4] = {0, };
	u08 erase[4] = {' ', ' ', ' ', ' '};
/*	
	Floor.upper = Floor.current + 1;
	Floor.lower = Floor.current - 1;
	if(Floor.upper == 10)
		Floor.upper++;
	else if(Floor.lower == 10)
		Floor.lower--;
*/
#ifndef _720p_use	
//	printf("video_format : %d \n", video_format);
	if((!video_format) || (video_format < 3) || (video_format > 6))			
#endif
	if(((osd_out_flag == 1) || (osd_out_flag == 2)) && pos)
	{
//		Rs485_Send(1);
//		printf("osd_out_flag : %d \n", osd_out_flag);
		switch(move_flag)
		{
			case MOVE_NONE:			osd_char_temp[3] = ' ';		break;						// Moving Stop		'  '
			case MOVE_ING_UP:		osd_char_temp[3] = 0x0a;	break;						// Moving Up		'£'
			case MOVE_ING_DOWN:		osd_char_temp[3] = 0x0b;	break;						// Moving Down		'¥'		
		}
		
		osd_char_temp[1] = osd_floor[Floor.current][0];
		osd_char_temp[2] = osd_floor[Floor.current][1];
//		printf("cur : %d, osd : %d \n", Floor.current, osd_char_temp[2] - '0');
/*			
		if((osd_char_temp[1] == 'X') && (osd_char_temp[2] == 'X') && move_flag == MOVE_ING_UP)
		{
			Floor.max = Floor.current;
			Floor.current--;
		}
		else if((osd_char_temp[1] == 'X') && (osd_char_temp[2] == 'X') && move_flag == MOVE_ING_DOWN)
		{
			Floor.min = Floor.current;
			Floor.current++;
		}
*/			
/*		if(door_step == 2)	
		{
			if(rule_flag == RULE_STOP)
				osd_char_temp[0] = door_data;
			else
				osd_char_temp[0] = 0x1F;
		}
*/
		if(door_step >= 2)				// always door chk
//			else if(door_step == 3)
			osd_char_temp[0] = door_data;
		else
			osd_char_temp[0] = ' ';
		
		Osd_Pos_Sel(pos);
		
		if(osd_out_flag == 2)
		{
			Osd_PutChar(PAGE_ENGLISH, erase, 4);
		}
		else if(osd_out_flag == 1)
		{
			Osd_PutChar(PAGE_ENGLISH, osd_char_temp, 4);
		}
		
		osd_out_flag = 0;
	}
}

void Floor_Reset_Display(void)
{
	if(Floor.current != Floor.base)
		Floor.current = Floor.base;
	osd_out_flag = 1;
}

//Serial_Menu
//-----------------------------------------------
void Serial_Value_Display(const u08 *ptr, u08 line)
{
	u08 i,cnt=0;
	u08 char_temp[2] = {0, };

	for(i=0;i<sizeof(Floor_Str);i++)	Term_Send(Floor_Str[i]);
	for(i=0;i<40;i++)	Term_Send(ptr[i]);
	Term_New_Line();

	for(i=0;i<sizeof(Osd_Str);i++)	Term_Send(Osd_Str[i]);
	while(cnt<10)
	{
		char_temp[0] = osd_floor[line + cnt][0];
		char_temp[1] = osd_floor[line + cnt][1];
		
		Term_Send(' ');
		Term_Send(char_temp[0]);
		Term_Send(char_temp[1]);
		Term_Send(' ');
		cnt++;
	}
	Term_New_Line();
	Term_New_Line();
}

void Serial_Menu_Display(void)
{
	u08 i;

	RS485_EN_HI;
	Term_Erase_Screen();
	Term_Erase_Screen();
	Term_Set_Cursor_Position(1,1);

	for(i=0;i<sizeof(Setup_Str);i++)	Term_Send(Setup_Str[i]);

	Term_New_Line();

	for(i=0;i<sizeof(Setup_Line1);i++)	Term_Send(Setup_Line1[i]);
	Term_New_Line();
	Term_New_Line();

//	for(i=0;i<sizeof(Config_View);i++)	Term_Send(Config_View[i]);	
//	Term_New_Line();	
	for(i=0;i<sizeof(Config_New);i++)	Term_Send(Config_New[i]);
	Term_New_Line();	
	for(i=0;i<sizeof(Config_ID);i++)	Term_Send(Config_ID[i]);
	Term_New_Line();	
	for(i=0;i<sizeof(Config_Reset);i++)	Term_Send(Config_Reset[i]);

	Term_New_Line();
	Term_New_Line();

	//for(i=0;i<48;i++)	Term_Send(Setup_Line1[i]);
	//Term_New_Line();
	
	for(i=0;i<sizeof(Config_Cmd);i++) 	Term_Send(Config_Cmd[i]);	
	Term_Set_Cursor_Position(8,6);

	Cursor = 0;
	Cursor_Pos_Row = 0;
	Cursor_Pos_Col = 0;
}
void Serial_Menu_1_Display(void)	// view current setting
{
	u08 i;

	Term_Erase_Screen();
	Term_Set_Cursor_Position(1,1);

	for(i=0;i<sizeof(Setup_Str);i++)	Term_Send(Setup_Str[i]);

	Term_New_Line();

	for(i=0;i<sizeof(Setup_Line1);i++)	Term_Send(Setup_Line1[i]);
	
	Term_New_Line();
	for(i=0;i<sizeof(ID_str);i++)	Term_Send(ID_str[i]);
	
	Term_Send(DectoAscii1(rs485_id[2]));
	Term_Send(DectoAscii1(rs485_id[1]));
	Term_Send(DectoAscii1(rs485_id[0]));
	
	Term_New_Line();
	for(i=0;i<sizeof(Setup_Line1);i++)	Term_Send(Setup_Line1[i]);
	
	Term_New_Line();
	for(i=0;i<11;i++)	Serial_Value_Display(Setup_Flr[i],i*10);
	
	Term_Set_Cursor_Position(MENU_ROW, MENU_COL);

	Cursor = 0;
	Cursor_Pos_Row = 0;
	Cursor_Pos_Col = 0;
}

void Serial_Menu_2_Display(void)
{
	u08 i;

	Term_Erase_Screen();
	Term_Set_Cursor_Position(1,1);

	for(i=0;i<sizeof(Setup_Str);i++)	Term_Send(Setup_Str[i]);

	Term_New_Line();

	for(i=0;i<sizeof(Setup_Line1);i++)	Term_Send(Setup_Line1[i]);
		
	Term_New_Line();
	for(i=0;i<sizeof(ID_str);i++)	Term_Send(ID_str[i]);
	Term_Set_Cursor_Position(3, 14);
	Term_Send(DectoAscii1(rs485_id[2]));
	Term_Send(DectoAscii1(rs485_id[1]));
	Term_Send(DectoAscii1(rs485_id[0]));
	Term_Set_Cursor_Position(3, 14);
}

void Serial_Menu_3_Display(void)
{
	u08 i;

	Term_Erase_Screen();
	Term_Set_Cursor_Position(1,1);

	for(i=0;i<sizeof(Setup_Str);i++)	Term_Send(Setup_Str[i]);

	Term_New_Line();

	for(i=0;i<sizeof(Setup_Line1);i++)	Term_Send(Setup_Line1[i]);
	
	Term_New_Line();
	for(i=0;i<sizeof(reset);i++)	Term_Send(reset[i]);
	Term_Set_Cursor_Position(3, 10);
}

void Serial_Menu_Proc(u08 rx_data)
{
	
	//printf("Arrow_Flag=%d Save_Flag=%d Pres_Flag=%d\r\n",Arrow_Flag, Save_Flag,Pres_Flag);
	if(Arrow_Flag)
	{
		Arrow_Flag = 0;

		switch(rx_data)																	// Ŀܭ Lտ
		{
			case 'A':						// UP
				if(Cursor_Pos_Row > 0)	Cursor_Pos_Row--;
				break;
			case 'B':						// DOWN
				if(Cursor_Pos_Row < 10)	Cursor_Pos_Row++;
				break;
			case 'C':						// RIGHT
				Cursor = 0;
				if(Cursor_Pos_Col < 9)	Cursor_Pos_Col++;
				break;
			case 'D':						// LEFT
				Cursor = 0;
				if(Cursor_Pos_Col > 0)	Cursor_Pos_Col--;
				break;
		}
		Term_Set_Cursor_Position(MENU_ROW + Cursor_Pos_Row*3, MENU_COL + Cursor_Pos_Col*4);

//		printf("Arrow_Flag => Cursor=0x%x Cursor_Pos_Row=0x%x Cursor_Pos_Col=0x%x\r\n", Cursor, Cursor_Pos_Row,Cursor_Pos_Col);
	}

	if(Save_Flag)
	{
		Save_Flag = 0;

		if((rx_data>0x60) && (rx_data<0x7b))	rx_data -= 0x20;						
		if((rx1.recv_flag == 1) && (rx_data != 0x08))
		{
			osd_floor[(Cursor_Pos_Row*10) + (Cursor_Pos_Col)][Cursor] = rx_data;
			Term_Send( rx_data );															
			if(Cursor == 0)	Cursor = 1;														
			else if(Cursor == 1)
			{
				if((Cursor_Pos_Row == 0) && (Cursor_Pos_Col == 0))						
				{
					if(osd_floor[0][0] == '-')
						Floor.base = 10 - (osd_floor[0][1]-0x30);
					else
						Floor.base = 10 + (10 * (osd_floor[0][0]-0x30)) + (osd_floor[0][1]-0x30);
				}
				if(Cursor_Pos_Col < 9)	Cursor_Pos_Col++;									// Ŀܭ 'ġ gvd
				else if(Cursor_Pos_Col == 9)
				{
					if(Cursor_Pos_Row < 10)	Cursor_Pos_Row++;
					else if(Cursor_Pos_Row == 10)	Cursor_Pos_Row = 0;
					Cursor_Pos_Col = 0;
				}
				Cursor = 0;
				Term_Set_Cursor_Position(MENU_ROW + Cursor_Pos_Row*3,MENU_COL + Cursor_Pos_Col*4);
				//printf("Save_Flag => Cursor=0x%x Cursor_Pos_Row=0x%x Cursor_Pos_Col=0x%x\r\n", Cursor, Cursor_Pos_Row,Cursor_Pos_Col);
			}
		}

	}
	
}void Serial_ID_Proc(u08 rx_data)
{
	
	if(Arrow_Flag)
	{
		Arrow_Flag = 0;
//		printf("id_cursor : %d \n", id_cursor);
		switch(rx_data)																	// Ŀܭ Lտ
		{
			case 'C':						// RIGHT
				Cursor = 0;
				if(id_cursor < 2)	id_cursor++;
				break;
			case 'D':						// LEFT
				Cursor = 0;
				if(id_cursor > 0)	id_cursor--;
				break;
		}
		Term_Set_Cursor_Position(3, 14 + id_cursor);
		//printf("Arrow_Flag => Cursor=0x%x Cursor_Pos_Row=0x%x Cursor_Pos_Col=0x%x\r\n", Cursor, Cursor_Pos_Row,Cursor_Pos_Col);
	}

	if(Save_Flag)
	{
		Save_Flag = 0;
	
		if((rx_data > 0x2f) && (rx_data < 0x3a))
		{
			Term_Send( rx_data );
			switch(id_cursor)
			{
				case 0:
					rs485_id[2] = rx_data - 0x30;
				break;
				case 1:
					rs485_id[1] = rx_data - 0x30;
				break;
				case 2:
					rs485_id[0] = rx_data - 0x30;
				Term_Set_Cursor_Position(3, 16);
				break;
			}
			if(id_cursor < 2)
				id_cursor++;
			CFG_Save_Sync_Off(PAGE1);
		}	
	}
}
void Menu_Reset(void)
{
	u08 i;
	
	Floor.base = DEFAULT_BASE_FLOOR;
	rs485_id[0] = DEFAULT_RS485_ID;
	rs485_id[1] = DEFAULT_RS485_ID;
	rs485_id[2] = DEFAULT_RS485_ID;
	
	for(i=0; i<110; i++)
		Default_FloorInfo_to_OSDInfo(i);
		
	CFG_Save_Sync_Off(PAGE1);
	CFG_Save_Sync_Off(PAGE2);
//	CFG_Read(PAGE1);
//	CFG_Read(PAGE2);
	
}
void Serial_Reset_Proc(u08 rx_data)
{
	if(Save_Flag)
	{
		Save_Flag = 0;

		if(rx_data == 'y')	rx_data -= 0x20;
		if(rx_data == 'Y')
		{
			Menu_Reset();
			
			Serial_Menu_Display();
			SysMode = SERIAL_MENU;
		}
	}
}

void Serial_Proc(u08 *rx_data, u08 rx_flag)
{
	if(rx_flag)
	{
		Set_Sync_Int(OFF, OFF);
		rx_flag = 0;
		switch(SysMode)
		{
			case SERIAL_MENU:
				Save_Flag = 0;
				if(rx_data[0] == 0x31) 											//key input is '1'
				{
					Serial_Menu_1_Display();
					SysMode = SERIAL_MENU_1;
				}
				else if(rx_data[0] == 0x32)										//key input is '2'
				{
					Serial_Menu_2_Display();
					SysMode = SERIAL_MENU_2;
				}
				else if(rx_data[0] == 0x33)
				{
					Serial_Menu_3_Display();
					SysMode = SERIAL_MENU_3;
					Save_Flag = 1;
				}
					
					
			break;
			case SERIAL_MENU_1:	
				Serial_Menu_Proc(rx_data[0]);										// ƍڌԎâ; ƫȑ ľǥރ OSD ܳd
			break;
			case SERIAL_MENU_2:
				Serial_ID_Proc(rx_data[0]);
			break;
			case SERIAL_MENU_3:
				Serial_Reset_Proc(rx_data[0]);
			break;
		}
		
		if(rx_data[0] == 0x08)												//key input is 'BS(backspace)'
		{
			Serial_Menu_Display();
			SysMode = SERIAL_MENU;
			CFG_Save_Sync_Off(PAGE1);
			CFG_Save_Sync_Off(PAGE2);
			id_cursor = 0;
		}
		Set_Sync_Int(OFF, ON);
	}
	rx1.recv_flag = 0;	
}

//Controller------------------------------------
void Fnd_Display(u08 ch_data)
{
	switch(ch_data)
	{
		case 0:
			FD3_OFF; FD2_OFF; FD1_OFF; FD0_OFF;
		break;
		case 1:
			FD3_OFF; FD2_OFF; FD1_OFF; FD0_ON;
		break;
		case 2:
			FD3_OFF; FD2_OFF; FD1_ON; FD0_OFF;
		break;
		case 3:
			FD3_OFF; FD2_OFF; FD1_ON; FD0_ON;
		break;
		case 4:
			FD3_OFF; FD2_ON; FD1_OFF; FD0_OFF;
		break;
		case 5:
			FD3_OFF; FD2_ON; FD1_OFF; FD0_ON;
		break;
		case 6:
			FD3_OFF; FD2_ON; FD1_ON; FD0_OFF;
		break;
		case 7:
			FD3_OFF; FD2_ON; FD1_ON; FD0_ON;
		break;
		case 8:
			FD3_ON; FD2_OFF; FD1_OFF; FD0_OFF;
		break;
		case 9:
			FD3_ON; FD2_OFF; FD1_OFF; FD0_ON;
		break;
		case 'x':
			FD3_ON; FD2_OFF; FD1_ON; FD0_OFF;
		break;
		case '-':
			FD3_ON; FD2_OFF; FD1_ON; FD0_OFF; FND_BAR_ON;
		break;
	}
}

void Fnd_Proc(void)
{
	FND_BAR_OFF;
	Fnd_Display(fnd_data[fnd_ch_cnt]);
	if(blink_flag[fnd_ch_cnt] == 1)			//off state(blink mode)
	{
		blink_cnt[fnd_ch_cnt]++;
		FND_BAR_OFF;
		Fnd_Display('x');

		if(blink_cnt[fnd_ch_cnt] == 15)
		{
			blink_flag[fnd_ch_cnt] = 2;
			blink_cnt[fnd_ch_cnt] = 0;
		}
	}
	else if(blink_flag[fnd_ch_cnt] == 2)	//on state(blink mode)
	{
		blink_cnt[fnd_ch_cnt]++;
		Fnd_Display(fnd_data[fnd_ch_cnt]);
		if(blink_cnt[fnd_ch_cnt] == 30)
		{
			blink_flag[fnd_ch_cnt] = 1;
			blink_cnt[fnd_ch_cnt] = 0;
		}
	}
//	printf("fnd_data[%d] : %d \n", fnd_ch_cnt, fnd_data[fnd_ch_cnt]);
	switch(fnd_ch_cnt)
	{
		case 0:
			FND_CH1_ON; FND_CH2_OFF; FND_CH3_OFF; FND_CH4_OFF; FND_CH5_OFF;
		break;
		case 1:
			FND_CH1_OFF; FND_CH2_ON; FND_CH3_OFF; FND_CH4_OFF; FND_CH5_OFF;
		break;
		case 2:
			FND_CH1_OFF; FND_CH2_OFF; FND_CH3_ON; FND_CH4_OFF; FND_CH5_OFF;
		break;
		case 3:
			FND_CH1_OFF; FND_CH2_OFF; FND_CH3_OFF; FND_CH4_ON; FND_CH5_OFF;
		break;
		case 4:
			FND_CH1_OFF; FND_CH2_OFF; FND_CH3_OFF; FND_CH4_OFF; FND_CH5_ON;
		break;
	}
	
	fnd_ch_cnt++;
	if(fnd_ch_cnt == 5)
		fnd_ch_cnt = 0;
}
u08 Fnd_F_Char(u08 F_temp)
{
	if(F_temp < 10)
		F_temp = '-';
	else
	{		
		F_temp/=10;
		F_temp-=1;
	}
	
	return F_temp;
}

u08 Fnd_B_Char(u08 B_temp)
{
	if(B_temp < 10)
		B_temp = 10 - B_temp;
	else
		B_temp%=10;
	
	return B_temp;
}

void Buzzer(u08 onoff_flag)
{
	if(onoff_flag)
		BUZZER_ON;
	else
		BUZZER_OFF;

}
void Buzzer_Check(void)
{
	if(buzzer_cnt_flag == 1)
	{
		buzzer_cnt++;
		if(buzzer_cnt == 3)
		{
			Buzzer(OFF);
			buzzer_cnt_flag = 0;
			buzzer_cnt = 0;
		}
	}
}
void Controller_Connect(void)
{
	Buzzer(ON);
	buzzer_cnt_flag = 1;
	ctrl_connect_flag = 3;
}

void Set_Key_Proc(void)
{
	ctrl_mode = MODE_DEFAULT;
	osd_out_flag = 1;
	ctrl_connect_flag = 3;
	fnd_num = 0;
	fnd_cursor = 0;
	Controller_Init();
	blink_flag[CH1] = 0;	blink_flag[CH2] = 0;	blink_flag[CH3] = 0;	blink_flag[CH4] = 0;	blink_flag[CH5] = 0;
}
void Contoller_Key_Proc(u08 key_val)
{
	u08 floor_temp = 0;
	u08 osd_temp = 0;
	switch(key_val)
	{
		case KEY_SET:
			ctrl_connect_flag = 2;
			switch(ctrl_mode)
			{
				case MODE_0_MENU:
					blink_flag[CH4] = 0;
					switch(fnd_data[CH4])
					{
						case 1:
							ctrl_mode = MODE_1_BASE;	// change Base Floor 	[x][x][][][y][y]	xx: current base floor / yy: floor to change for base floor
							fnd_data[CH1] = Fnd_F_Char(Floor.base);
							fnd_data[CH2] = Fnd_B_Char(Floor.base);
							fnd_data[CH3] = 'x';
							fnd_data[CH4] = 0;
							fnd_data[CH5] = 0;
							fnd_cursor = CH4;
							fnd_max_cursor = CH5;
							fnd_min_cursor = CH3;
							blink_flag[fnd_cursor] = 2;
							
						break;
						case 2:
							ctrl_mode = MODE_2_OSD;		// change OSD char 		[x][x][][y][y][y]	xx: floor to change code / yyy: OSD char code
							fnd_data[CH1] = 0;
							fnd_data[CH2] = 0;
							fnd_data[CH3] = 0;
							fnd_data[CH4] = 0;
							fnd_data[CH5] = 0;
							fnd_cursor = CH1;
							fnd_max_cursor = CH5;
							fnd_min_cursor = CH1;
							blink_flag[fnd_cursor] = 2;
						break;
						case 3:
							ctrl_mode = MODE_3_ID;		// set RS485 ID	[][][][x][x][x]	xxx: RS485 ID(Device ID)
							fnd_data[CH1] = 'x';
							fnd_data[CH2] = 'x';
							fnd_data[CH3] = rs485_id[2];
							fnd_data[CH4] = rs485_id[1];
							fnd_data[CH5] = rs485_id[0];
							fnd_cursor = CH3;
							fnd_max_cursor = CH5;
							fnd_min_cursor = CH3;
							blink_flag[fnd_cursor] = 2;
						break;
						case 4:
							ctrl_mode = MODE_4_RESET;	// return Default Setting
							fnd_data[CH1] = 'x';
							fnd_data[CH2] = 'x';
							fnd_data[CH3] = 'x';
							fnd_data[CH4] = 'x';
							fnd_data[CH5] = 0;
							fnd_cursor = CH5;
							blink_flag[fnd_cursor] = 2;
						break;
					}
				break;
					
				case MODE_1_BASE:
					if(((fnd_data[CH4] == '-') && (!fnd_data[CH5]))|| ((fnd_data[CH4] == 0) && (fnd_data[CH5] == 0)))	// if -0, 00, base floor = 1F
						Floor.base = DEFAULT_BASE_FLOOR;
					else if(fnd_data[CH4] == '-')
						Floor.base = (10 - fnd_data[CH5]);
					else
					{
						Floor.base = (fnd_data[CH4] + 1) * 10;
						Floor.base += fnd_data[CH5];
					}
					CFG_Save_Sync_Off(PAGE1);
					Set_Key_Proc();
				break;
				
				case MODE_2_OSD:
					if((fnd_data[CH1] == '-') && (fnd_data[CH2])) 	// if -0, 00 -> x
						floor_temp = (10 - fnd_data[CH2]);
					else
					{
						floor_temp = (fnd_data[CH1] + 1) * 10;
						floor_temp += fnd_data[CH2];
					}
																	//				   10	1
					osd_temp = fnd_data[CH3]*10 + fnd_data[CH4];	// osd front char(|ch3|ch4|)
					if(osd_temp < 10)
						osd_floor[floor_temp][0] = osd_temp + 0x30;	// 0 ~ 9 -> osd (0 ~ 9)
					else if(osd_temp < 36)
						osd_floor[floor_temp][0] = osd_temp + 0x37;	// 10 ~ 35 -> osd (A ~ Z)
					
					if(fnd_data[CH5] == '-')						// if ch5 = '-', osd x	(osd back char) 
						osd_floor[floor_temp][1] = ' ';
					else
						osd_floor[floor_temp][1] = fnd_data[CH5] + 0x30;
					CFG_Save_Sync_Off(PAGE2);
					Set_Key_Proc();
				break;
				
				case MODE_3_ID:
					rs485_id[2] = fnd_data[CH3];
					rs485_id[1] = fnd_data[CH4];
					rs485_id[0] = fnd_data[CH5];
					CFG_Save_Sync_Off(PAGE1);
					Set_Key_Proc();
				break;
				
				case MODE_4_RESET:
					Menu_Reset();
					Set_Key_Proc();
				break;					
			}
			floor_temp = 0;
		break;
			
		case KEY_UP:
			switch(ctrl_mode)
			{
				case MODE_0_MENU:
					fnd_data[CH4]++;
					if(fnd_data[CH4] > 4)
						fnd_data[CH4] = 1;
				break;
				
				case MODE_1_BASE:	
					fnd_num++;
					if(fnd_cursor == CH4)
					{
						if(fnd_num > 10)
							fnd_num = 0;
						if(fnd_num == 10)
							fnd_data[fnd_cursor] = '-';
						else
							fnd_data[fnd_cursor] = fnd_num;
					}
					else
					{
						if(fnd_num > 9)
							fnd_num = 0;
						fnd_data[fnd_cursor] = fnd_num;
					}
				break;
				
				case MODE_2_OSD:
					fnd_num++;	
					if((fnd_cursor == CH1) || (fnd_cursor == CH5))
					{
						if(fnd_num > 10)
							fnd_num = 0;
						if(fnd_num == 10)
							fnd_data[fnd_cursor] = '-';
						else
							fnd_data[fnd_cursor] = fnd_num;
					}
					else
					{
						if(fnd_num > 9)
							fnd_num = 0;
						fnd_data[fnd_cursor] = fnd_num;
					}			
				break;
					
				case MODE_3_ID:
					fnd_num++;
					if(fnd_num > 9)
						fnd_num = 0;
					fnd_data[fnd_cursor] = fnd_num;
				break;
					
				case MODE_4_RESET:	
					if(fnd_data[CH5])
						fnd_data[CH5] = 0;
					else
						fnd_data[CH5] = 1;
				break;	
			}
		break;
		
		case KEY_DOWN:
			switch(ctrl_mode)
			{
				case MODE_0_MENU:
					fnd_data[CH4]--;
					if(fnd_data[CH4] == 0)
						fnd_data[CH4] = 4;
				break;
				
				case MODE_1_BASE:	
					fnd_num--;
					if(fnd_cursor == CH4)
					{
						if(fnd_num == 255)
							fnd_num = 10;
						if(fnd_num == 10)
							fnd_data[fnd_cursor] = '-';
						else
							fnd_data[fnd_cursor] = fnd_num;
					}
					else
					{
						if(fnd_num == 255)
							fnd_num = 9;
						fnd_data[fnd_cursor] = fnd_num;
					}
				break;
				
				case MODE_2_OSD:	
					fnd_num--;	
					if((fnd_cursor == CH1) || (fnd_cursor == CH5))
					{
						if(fnd_num == 255)
							fnd_num = 10;
						if(fnd_num == 10)
							fnd_data[fnd_cursor] = '-';
						else
							fnd_data[fnd_cursor] = fnd_num;
					}
					else
					{
						if(fnd_num == 255)
							fnd_num = 9;
						fnd_data[fnd_cursor] = fnd_num;
					}			
				break;
					
				case MODE_3_ID:	
					fnd_num--;
					if(fnd_num == 255)
						fnd_num = 9;
					fnd_data[fnd_cursor] = fnd_num;
				break;
					
				case MODE_4_RESET:	
					if(fnd_data[CH5])
						fnd_data[CH5] = 0;
					else
						fnd_data[CH5] = 1;
				break;	
			}
		break;
			
		case KEY_RIGHT:
			switch(ctrl_mode)
			{
				case MODE_1_BASE:
				case MODE_2_OSD:	
				case MODE_3_ID:	
					if(fnd_cursor < fnd_max_cursor)
					{
						blink_flag[fnd_cursor] = 0;
						fnd_cursor++;
						if(fnd_data[fnd_cursor] == 'x')
							fnd_cursor++;
						blink_flag[fnd_cursor] = 2;
						if(fnd_data[fnd_cursor] == '-')
							fnd_num = 10;
						else
							fnd_num = fnd_data[fnd_cursor];
					}
				break;
			}
		break;
			
		case KEY_LEFT:
			switch(ctrl_mode)
			{
				case MODE_1_BASE:	
				case MODE_2_OSD:	
				case MODE_3_ID:	
					if(fnd_cursor > fnd_min_cursor)
					{
						blink_flag[fnd_cursor] = 0;
						fnd_cursor--;
						if(fnd_data[fnd_cursor] == 'x')
							fnd_cursor--;
						blink_flag[fnd_cursor] = 2;
						if(fnd_data[fnd_cursor] == '-')
							fnd_num = 10;
						else
							fnd_num = fnd_data[fnd_cursor];
					}
				break;
			}
		break;
		
		case KEY_MODE:
			if(ctrl_mode == MODE_0_MENU)
			{
				Set_Key_Proc();
				blink_flag[CH4] = 0;
				fnd_num = 0;
			}
			else
			{
				ctrl_connect_flag = 2;
				blink_flag[CH1] = 0;	blink_flag[CH2] = 0;	blink_flag[CH3] = 0;	blink_flag[CH4] = 0;	blink_flag[CH5] = 0;
				ctrl_mode = MODE_0_MENU;
				fnd_data[CH1] = 'x';
				fnd_data[CH2] = 'x';
				fnd_data[CH3] = '-';
				fnd_data[CH4] = 1;
				fnd_data[CH5] = '-';
				fnd_cursor = CH4;
				blink_flag[fnd_cursor] = 2;
			}
			
		break;
	}
}
void Controller_Init(void)
{
	ctrl_mode = MODE_DEFAULT;
	fnd_data[CH1] = 'x';
	fnd_data[CH2] = 'x';
	fnd_data[CH3] = 'x';
	fnd_data[CH4] = Fnd_F_Char(Floor.current);
	fnd_data[CH5] = Fnd_B_Char(Floor.current);
}

void Controller_proc(void)
{
	if(ctrl_connect_flag == 1)
		Controller_Connect();
	else if(ctrl_connect_flag == 3)
		Controller_Init();
	
	if(DrvGPIO_GetBit(E_GPA, CTRLER_LINK) && ctrl_connect_flag)
	{
		ctrl_connect_flag = 0;
//		printf("ctrl_ unconnected\n");
	}
	else if((!DrvGPIO_GetBit(E_GPA, CTRLER_LINK)) && (ctrl_connect_flag == 0))
	{
		ctrl_connect_flag = 1;
//		printf("ctrl_ connected\n");
	}
}


//--------------------------------------------------------------------------------------
void Boot_Display(void)
{
	Utc_Video_Check();
	Osd_Erase(row_data);
	delay_ms(1);
	Osd_Locate(2, 2);
	delay_ms(1);
#ifndef _720p_use	
	if((video_format == 0) || (video_format < 3) || (video_format > 6))			
#endif
	Osd_PutStr(PAGE_ENGLISH, (u08*)"SC-EFI04");
}
void System_Reset(void)
{
#ifdef _DBG_PRINT_
	printf("System Reset!!!\n");
#endif

	delay_ms(20);

//	IPRSTC1 = 1;
	DrvSYS_ResetChip();
}

void System_Init(void)
{
#ifdef _DBG_PRINT_
	dbg_start();
#endif
	
	CFG_Init();
	UART_Init(RS485_PORT, BAUD19_2K);
	I2C_Init();
	TMR0_Init();
	UTC_Init();
//	Boot_Display();
	Serial_Menu_Display();
	ADC_Init();
	ADC_En_Ch(UP);
	
}

void Boot_Init(void)
{
//	printf("osd_position: %d \n", osd_position);
	Osd_Erase(row_data);
	if(osd_position)
		osd_out_flag = 3;
	
	if((door_max != 0) && (door_min != 0))
		door_step = 2;
//	printf("door_step: %d \n", door_step);
	LASER_ZIG_OFF;
	rule_flag = RULE_NONE;
	move_flag = MOVE_NONE;
	BUZZER_OFF;

}


void Rs485_Send(u08 mode)
{
	u08 txd[8] = {0, };
	u08 i = 0;
	txd[0] = START_CODE;
	txd[1] = DEVICE_CODE;
	txd[2] = (rs485_id[2] * 100) + (rs485_id[1] * 10) + rs485_id[0];
	txd[3] = osd_position;
	txd[4] = Floor.current;
	
	if(mode)
	{
		switch(move_flag)
		{
			case MOVE_NONE:			txd[5] = 0;	break;				
			case MOVE_ING_UP:		txd[5] = 1;	break;	
			case MOVE_ING_DOWN:		txd[5] = 2;	break;			
		}
		
		txd[6] = door_rs_data;
	}
	else
	{
		txd[5] = test_move;
		txd[6] = test_door;
	}
	
	txd[7] = txd[2] + txd[3] + txd[4] + txd[5] + txd[6];	
	
	if(no_change_flag)
	{
		uart_send(UART_PORT1, txd, 8);
		for(i = 3; i < 8; i++)
		{
			if(pre_485_stat[i] != txd[i])
			{
				memcpy(pre_485_stat, txd, 8);
				break;
			}
		}
		//printf("RS485 SEND\n");
		no_change_flag = 0;
	}
	else
	{
		for(i = 3; i < 8; i++)
		{
			if(pre_485_stat[i] != txd[i])
			{
				//printf("%02x %02x %02x %02x %02x %02x %02x %02x\n", txd[0], txd[1], txd[2], txd[3], txd[4], txd[5], txd[6], txd[7]);
				uart_send(UART_PORT1, txd, 8);
				memcpy(pre_485_stat, txd, 8);
				break;
			}
		}
	}
}

#ifdef TEST_MODE
void Test_Proc(void)
{
	osd_out_flag = 2;
	Test_Floor_Display(osd_position);
	
	Floor.current++;
	if(Floor.current == 10)
		Floor.current++;

	if(Floor.current > 109)
		Floor.current = 1;
	
	osd_position++;
	if(osd_position == 7)
		osd_position = 0;
	else if(osd_position)
		osd_out_flag = 1;
	
	test_move++;
	if(test_move > 2)
		test_move = 0;
	
	test_door++;
	if(test_door > 2)
		test_door = 0;
		
	Test_Floor_Display(osd_position);
	Rs485_Send(0);
}
                     

void Test_Key_Proc(void)
{
	u08 key_val = 0;
	key_val = Key_Check();
	

	switch(key_val)
	{
		case (KEY_LASER | KEY_OSD):
			test_mode = 0;
				
			Osd_Clear();
			delay_ms(10);
			CFG_Read(PAGE1);
			delay_ms(10);
			Floor.current = 11;
			osd_out_flag = 1;
			Floor_Display(osd_position);
			
		break;
	}
}


void Display_TEST(void)
{
	delay_ms(2);
	switch(video_format)
	{
		case SD_PAL:	case SD_NTSC:
			Osd_Locate(6,9);
		break;	
#ifdef _720p_use		
		case HD_720P25:	case HD_720P30:	case HD_720P50:	case HD_720P60:
			
			if(video_signal == TVI)
				Osd_Locate(8,4);
			else if(video_signal == CVI)
			{
				if(video_format == HD_720P30)
					Osd_Locate(8,8);	
				else
					Osd_Locate(8, 14);
			}
			else if(video_signal == AHD)
				Osd_Locate(8,8);
		break;
#endif
		case HD_1080P25:	case HD_1080P30:
			
		if(video_signal == TVI)
			Osd_Locate(8,9);
		else if(video_signal == CVI)
		{
			if(video_format == HD_1080P30)
				Osd_Locate(8,9);	
			else
				Osd_Locate(8,11);
		}
		else if(video_signal == AHD)
			Osd_Locate(8,9);
		break;
		case UHD_3MP:
			
		break;
		case UHD_4MP:
			
		break;
	}
	
	delay_ms(1);
	
	Osd_PutStr(PAGE_ENGLISH, (u08*)"TEST MODE");
	
	delay_ms(1);
	
}


void Test_Floor_Display(u08 pos)
{
	u08 osd_char_temp[4] = {0, };
	u08 erase[4] = {' ', ' ', ' ', ' '};

#ifndef _720p_use	
//	printf("video_format : %d \n", video_format);
	if((!video_format) || (video_format < 3) || (video_format > 6))			
#endif
	if(((osd_out_flag == 1) || (osd_out_flag == 2)) && pos)
	{
		
//		printf("osd_out_flag : %d \n", osd_out_flag);
		switch(test_move)
		{
			case 0:		osd_char_temp[3] = ' ';		break;						// Moving Stop		'  '
			case 1:		osd_char_temp[3] = 0x0a;	break;						// Moving Up		'£'
			case 2:		osd_char_temp[3] = 0x0b;	break;						// Moving Down		'¥'		
		}
		
		osd_char_temp[1] = osd_floor[Floor.current][0];
		osd_char_temp[2] = osd_floor[Floor.current][1];

		switch(test_door)
		{
			case 0:		osd_char_temp[0] = ' ';		break;						// Moving Stop		'  '
			case 1:		osd_char_temp[0] = 0x1E;	break;						// Moving Up		'£'
			case 2:		osd_char_temp[0] = 0x1F;	break;						// Moving Down		'¥'		
		}
		
		Osd_Pos_Sel(pos);
		
		if(osd_out_flag == 2)
		{
			Osd_PutChar(PAGE_ENGLISH, erase, 4);
		}
		else if(osd_out_flag == 1)
		{
			
			Osd_PutChar(PAGE_ENGLISH, osd_char_temp, 4);
		}
		
		osd_out_flag = 0;
	}
}
#endif
//======================================================================
int main(void)
{
	MCU_Init();
	System_Init();
	Boot_Init();
	
#ifdef _DBG_PRINT_
	printf("-----------main loop------------\n");
#endif
		
	while(1)
	{
#ifdef TEST_MODE		
		if(test_mode)
		{
			if(tFlag_10ms)
			{
				Test_Key_Proc();
				tFlag_10ms = 0;
			}
			
			if(tFlag_3sec)	/// 3sec setting now
			{
				Test_Proc();
				tFlag_3sec = 0;
			}
		}
		else
#endif
		{
			if(sensor_int_flag)
			{
				cur_sensor = DrvGPIO_GetBit(E_GPB, 9);			//PRE: Previous, CUR: Current
				cur_sensor |= (DrvGPIO_GetBit(E_GPB, 10)) <<1;
				sensor_int_flag = 0;
			}
			
			if(tFlag_10ms)
			{
				Key_Proc();
				if(osd_boot_flag)
					PhotoSensor_Check();
				Floor_Display(osd_position);
				Hsync_Chk_Proc();
//				Rs485_Send(1);
				tFlag_10ms = 0;
			}

			if(tFlag_100ms)
			{
				Video_Resolution_Check();
				ID_Off_check();
				ZigOff_Check();
				Buzzer_Check();
				if(id_flag == 1)
					Display_ID();
				Rs485_Send(1);
				tFlag_100ms = 0;
			}
			
			if(tFlag_200ms)
			{
				Loss_Change_Chk();
				TmrCntSrv_Proc();
				Door_Data_Check();
				Controller_proc();
				tFlag_200ms = 0;
			}
			
			if(tFlag_1sec)
			{
				//no_change_flag = 1;
				tFlag_1sec = 0;
			}
		}
		
#ifdef _USE_UART0_
		if(rx0.recv_flag){ Uart_Proc(rx0.data, rx0.recv_flag);	rx0.recv_flag = 0; }
#endif

#ifdef _USE_UART1_
		if(rx1.recv_flag)	Serial_Proc(rx1.buff, rx1.recv_flag);
#endif
	}
}
