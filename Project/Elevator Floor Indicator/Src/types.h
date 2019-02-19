/*
 * types.h
 * define data types.
 *
 * Copyright (c) 2012, SeeEyes Co.,Ltd.
 * Harold Choi.
 *
 * Log:
 * 
 * Revision 0.1  2012/04/25  12:11:00  Harold Choi
 * First Release
 * 
 */

#ifndef _TYPES2_H_
#define _TYPES2_H_

typedef char int8;
typedef volatile char vint8;
typedef unsigned char uint8;
typedef volatile unsigned char vuint8;
typedef short int16;
typedef volatile short vint16;
typedef unsigned short uint16;
typedef volatile unsigned short vuint16;
typedef long int32;
typedef volatile long vint32;
typedef unsigned long uint32;
typedef volatile unsigned long vuint32;

typedef uint8 SOCKET;

typedef unsigned char  u8;
typedef unsigned short u16;
typedef unsigned long  u32;
typedef char           s8;
typedef short          s16;
typedef long           s32;
typedef unsigned long long u64;
typedef long long      s64;

typedef unsigned char   bool;


/*
typedef unsigned char	__u8;
typedef unsigned short	__u16;
typedef unsigned long	__u32;
typedef char			__s8;
typedef short			__s16;
typedef long			__s32;
typedef unsigned short	__le16;
typedef unsigned long	__le32;
typedef unsigned short	__be16;
typedef unsigned long	__be32;
*/

#define	BIT0		0x01
#define	BIT1		0x02
#define	BIT2		0x04
#define	BIT3		0x08
#define	BIT4		0x10
#define	BIT5		0x20
#define	BIT6		0x40
#define	BIT7		0x80

#define	N_BIT0		0xFE
#define	N_BIT1		0xFD
#define	N_BIT2		0xFB
#define	N_BIT3		0xF7
#define	N_BIT4		0xEF
#define	N_BIT5		0xDF
#define	N_BIT6		0xBF
#define	N_BIT7		0x7F


#define	ON			1
#define	OFF			0

#define	YES			1
#define	NO			0

#define	HIGH		1
#define	LOW			0

#define	NTSC		0
#define	PAL			1

////////////////////////////////////////////////////////////
// For Funtion Factor
////////////////////////////////////////////////////////////
#define IN
#define OUT
#define INOUT
////////////////////////////////////////////////////////////

//----------------------------------------------------------------------
typedef enum{
	FALSE,
	TRUE
} BOOLEAN;

union DW_TEMP{
	u8	valB[4];	//B[]={0x0xaa,0xbb,0xcc,0xdd}
	u16	valW[2];	//W[]={0xbbaa,0xddcc}
	u32	valDW;		//DW={0xddccbbaa}
};


#define	SET_BIT(addr,bit)	(addr |= (1 << bit))
#define	CLR_BIT(addr,bit)	(addr &= (~(1 << bit)))

#define	INV_BIT(addr,bit)	(addr ^= (1 << bit))
#define	GET_BIT(reg,bit)	((reg >> bit) & 1)

#define	HextoAsciiHi(x)		((x>>4)<10)?((x>>4)+0x30):((x>>4)+0x37)
#define	HextoAsciiLo(x)		((x&0x0f)<10)?((x&0x0f)+0x30):((x&0x0f)+0x37)

#define	DectoAsciiHi(x)		((x/10)|0x30)
#define	DectoAsciiLo(x)		((x%10)|0x30)


#define	DectoAscii6(x)		(((x%1000000)/10000)+0x30)
#define	DectoAscii5(x)		(((x%100000)/1000)+0x30)
#define	DectoAscii4(x)		(((x%10000)/1000)+0x30)
#define	DectoAscii3(x)		(((x%1000)/100)+0x30)
#define	DectoAscii2(x)		(((x%100)/10)+0x30)
#define	DectoAscii1(x)		(((x%10)/1)+0x30)

#define	DectoHex(x)			((x<10)?(x+0x30):(x+0x37))
#define AsciltoDec(x)		(x-0x30)
#define DectoBcd(x)			(((x/10)*16)+(x%10))
#define BcdtoDec(x)			(((x/16)*10)+(x%16))
#endif		/* _TYPES2_H_ */

