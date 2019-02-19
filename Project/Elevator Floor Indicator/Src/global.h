#ifndef __GLOBAL_H__
#define __GLOBAL_H__

typedef unsigned char		u08;
typedef   		 char		s08;
typedef unsigned short		u16;
typedef   		 short		s16;
typedef unsigned int		u32;
typedef   		 int		s32;
//typedef unsigned long		u32;
//typedef   		 long	 	s32;

typedef const unsigned char	c08;
typedef const unsigned int	c32;
#define	ON			1
#define	OFF			0

#define	LOW			0
#define	HIGH		1

#define	YES			1
#define	NO			0

#define	NTSC		0
#define	PAL			1

//----------------------------------------------------------------------

union DW_TEMP{
	u08	valB[4];	//B[]={0x0xaa,0xbb,0xcc,0xdd}
	u16	valW[2];	//W[]={0xaabb,0xccdd}
	u32	valDW;		//DW=0xaabbccdd
};


#define	SET_BIT(addr,bit)	(addr |= (1 << bit))
#define	CLR_BIT(addr,bit)	(addr &= (~(1 << bit)))

#define	INV_BIT(addr,bit)	(addr ^= (1 << bit))
#define	GET_BIT(reg,bit)	((reg >> bit) & 1)

#define	HextoAsciiHi(x)		((x>>4)<10)?((x>>4)+0x30):((x>>4)+0x37)
#define	HextoAsciiLo(x)		((x&0x0f)<10)?((x&0x0f)+0x30):((x&0x0f)+0x37)

#define	DectoAsciiHi(x)		((x/10)|0x30)
#define	DectoAsciiLo(x)		((x%10)|0x30)

#define	DectoAscii3(x)		((x/100)+0x30)
#define	DectoAscii2(x)		(((x%100)/10)+0x30)
#define	DectoAscii1(x)		((x%10)+0x30)


#define	DectoHex(x)			((x<10)?(x+0x30):(x+0x37))
#define HextoDec(x)			((x>29) && (x<3A))? (x-0x30) : (x-0x37)

#endif
