#ifndef _KEY__H_
#define _KEY__H_

////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Pin Map & Ctrl Definition
//#define LED_SDI			12	// PA12		// (O)		// LED_DS
//#define LED_SRCLK			13	// PA13		// (O)		// LED_SH
//#define LED_RCLK			14	// PA14		// (O)		// LED_ST
#define	LED_SDI				LED_DS
#define LED_SRCLK			LED_SH
#define LED_RCLK			LED_ST

#define	KEY_DELAY			delay_us(1)

#define	LED_SDI_HI			DrvGPIO_SetBit(E_GPC,LED_SDI)
#define	LED_SDI_LO			DrvGPIO_ClrBit(E_GPC,LED_SDI)
#define	LED_RCLK_HI			DrvGPIO_SetBit(E_GPC,LED_RCLK)
#define	LED_RCLK_LO			DrvGPIO_ClrBit(E_GPC,LED_RCLK)
#define	LED_RCLK_TGL		LED_RCLK_HI; KEY_DELAY; LED_RCLK_LO
#define	LED_SRCLK_HI		DrvGPIO_SetBit(E_GPC,LED_SRCLK)
#define	LED_SRCLK_LO		DrvGPIO_ClrBit(E_GPC,LED_SRCLK)
#define	LED_SRCLK_TGL		KEY_DELAY; LED_SRCLK_HI; KEY_DELAY; LED_SRCLK_LO

	
#define LED_CH4_POW			0x0001	
#define	LED_CH4_LOCK		0x0002
#define LED_CH3_POW			0x0004
#define	LED_CH3_LOCK		0x0008
#define LED_CH2_POW			0x0010
#define	LED_CH2_LOCK		0x0020
#define LED_CH1_POW			0x0040
#define	LED_CH1_LOCK		0x0080
#define LED_TDM_POW		    0x0100
#define	LED_TDM_LOCK	    0x0200



extern u16 Led_On_Key;
extern u08 Led_On_Cnt;

void LED_proc(void);
void LED_Set(u08 ch, u08 stat);

void LED_Init(void);
void LED_ON_Set(u16 LEDs);
void LED_Inverse_Set(u16 LEDs);
u16 LOCK_LED_BIT(u08 ch);
void LED_Blinking_Set(u16 LEDs,u08 Speed);
void LED_Shift_When_System_Start(void);
// -------------------------------------------------
#endif
