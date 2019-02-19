//----------------------------------------------------------------------------------//
//
//----------------------------------------------------------------------------------//
#include <stdio.h>


#include "DrvGPIO.h"
#include "common.h"
#include "main.h"
#include "LED.h"


u16 Led_On_Key = 0;
u08 Led_On_Cnt = 0;

////////////////////////////////////////////////////////////////////////////////////////////////////////
u08 Blinking_Sync=0;	// All LED On/OFF Timing Sync

void LED_ON_Set(u16 LEDs)
{
	Led_On_Key |= LEDs;
}

void LED_OFF_Set(u16 LEDs)
{
	Led_On_Key &= ~LEDs;
}

void LED_Inverse_Set(u16 LEDs)
{
	Led_On_Key ^= LEDs;
}

void LED_Blinking_Set(u16 LEDs,u08 Speed)// Speed: 1,4,5,6  1:Slow(600ms),4:(300ms),5:(200ms),6:Fast(100ms)
{
	if(((Blinking_Sync/(7-Speed))%2)==0)	LED_ON_Set(LEDs);
	else				LED_OFF_Set(LEDs);
}

u16 LOCK_LED_BIT(u08 ch)
{
	if(ch==NUMBER_OF_POC-1)	return LED_TDM_LOCK;
	else	return (LED_CH1_LOCK>>(ch*2));
}

u16 POWER_LED_BIT(u08 ch)
{
	if(ch==NUMBER_OF_POC-1)	return LED_TDM_POW;
	else	return (LED_CH1_POW>>(ch*2));
}

void LED_Blinking_Dynamic(u08 ch, u08 Long)
{
	if(Long)LED_Blinking_Set(POWER_LED_BIT(ch),1);
	else	LED_Blinking_Set(POWER_LED_BIT(ch),4);
}
u08 LED_Shift_Flag_When_System_Start=1;
void LED_proc(void)
{
	u08 i;
	u16 tmp;
	static u08 Long=1;

	if(LED_Shift_Flag_When_System_Start)LED_Shift_When_System_Start();
	else
	{
		for(i=0;i<NUMBER_OF_POC;i++)
		{
			if(ChStat[i]) LED_ON_Set(LOCK_LED_BIT(i));
			else	LED_Blinking_Set(LOCK_LED_BIT(i),1);
			if(Vout_Ctrl_Flag & (0x01<<i))
			{
				if(Pwr_Stat_Ch[i] != PWR_CHK)	LED_Blinking_Dynamic(i,Long);
				else if(C_Stat[i] == SHORT)		LED_Blinking_Set(POWER_LED_BIT(i),6);
				else if((C_Stat[i]==OPEN)||(C_Stat[i]==OVER))	LED_Blinking_Set(POWER_LED_BIT(i),4);
				else LED_ON_Set(POWER_LED_BIT(i));
			}
			else	LED_OFF_Set(POWER_LED_BIT(i));
		}
	}

	//led On/Off
	tmp = Led_On_Key & 0x03ff;

	for (i=0;i<10;i++){
		(tmp&(0x0001<<i))?LED_SDI_LO:LED_SDI_HI;
		LED_SRCLK_TGL;
	}
	LED_RCLK_TGL;
	LED_SDI_LO;

	if(Blinking_Sync==11)
	{
		Blinking_Sync=0;
		if(Long)Long=0;
		else Long=1;
	}
	else Blinking_Sync++;
}

void LED_Shift_When_System_Start(void)
{
	static u08 i=0;
	switch(i)
	{
		case 0:			Led_On_Key=LED_TDM_LOCK;			break;
		case 5:			Led_On_Key=LED_TDM_POW;				break;
		case 1:case 2:case 3:case 4:case 6:case 7:case 8:case 9:
			 Led_On_Key=Led_On_Key>>2;
			 break;

		case 10:		Led_On_Key=LED_CH4_LOCK;			break;
		case 15:		Led_On_Key=LED_CH4_POW;				break;
		case 11:case 12:case 13:case 14:case 16:case 17:case 18:case 19:
			Led_On_Key=Led_On_Key<<2;
			break;

		case 20:LED_Shift_Flag_When_System_Start=0; Led_On_Key=0; break;
	}
	i++;
}
void LED_Set(u08 ch, u08 stat)
{
	switch(ch)
	{
		case 0: break;
		case 1: break;
		case 2: break;
		case 3: break;
	}
}

//==================================================================================
//	KEY CHECK & PROCEDURE
//==================================================================================
void LED_Init(void)
{
	Led_On_Key = 0x01;
	LED_proc();
}



