#ifndef _CFG_DEF_H_
#define _CFG_DEF_H_

/*
User Configuration
Config0 (Address = 0x0030_0000)

Bits	Descriptions
[31:29]	Reserved
[28]	CKF			XT1 Clock Filter Enable				0 = Disable, 1 = Enable
[27]	Reserved
[26:24]	CFOSC		CPU Clock Source Select				000 = Ext 2~24MHz
														111 = Int 22.1184MHz
														xxx = Reserved
[23]	CBODEN		Brown-Out Dectector Enable			0 = Enable,  1 = Disable
[22:21]	CBOV1_0		Brown-Out Voltage Selection			00 = 2.2V
														01 = 2.7V
														10 = 3.8V
														11 = 4.5V
[20]	CBORST		Brown-Out Reset Enable				0 = Enable,  1 = Disable
[19:8]	Reserved
[7]		CBS			Chip Boot Selection					0 = LDROM,   1 = APROM
[6:2]	Reserved
[1]		LOCK		Security Lock						0 = locked,  1 = not locked
[0]		DFEN		Data Flash Enable(only for 128KB)	0 = Enable,  1 = Disable


31	30	29|	28|		27|	26	25	24|		23|	22	21|	20|		19	18	17	16
---------------------------------------------------------------------------
 1	 1	 1	 1		 1	 0	 0	 0		 0	 0	 1	 0		 1	 1	 1	 1


15	14	13	12		11	10	 9	 8|		 7|	 6	 5	 4		 3	 2|	 1|	 0
---------------------------------------------------------------------------
 1	 1	 1	 1		 1	 1	 1	 1		 0	 1	 1	 1		 1	 1	 1	 0

CFG Set
 - CLOCK SELECT		: Ext 12.288M (for I2S MCLK)
 - BOD RESET		: Reset Enable, 2.7V
 - BOOT SELECT		: LDROM
 - SECURITY LOCK	: Enable
 - DATA FLASH		: Enable
*/
#define DATAFLASH_BASE_ADDR		0x0001F000

//#define CFG0_SET_VAL	0xf2fff7c		//0xff2fff7e
#define CFG0_SET_VAL	0xf82fff7e		

#define CFG1_SET_VAL	DATAFLASH_BASE_ADDR		// 0x0001F000

#define CFG_CHK_CODE	0x87654321
#define CFG_CHK_CODE1	0x21
#define CFG_CHK_CODE2	0x43
#define CFG_CHK_CODE3	0x65
#define CFG_CHK_CODE4	0x87

#endif
