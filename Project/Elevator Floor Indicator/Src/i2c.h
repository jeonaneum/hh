#ifndef __I2C_H__
#define __I2C_H__

#define USE_SOFTWAREI2C		1
#define USE_HARDWAREI2C		2

#define	I2CMODE				USE_SOFTWAREI2C

#define _I2C0_ENABLE_
//#define _I2C1_ENABLE_

// pin assignments
#define SDA0					8			// PA8	// i2c data pin
#define SCL0					9			// PA9	// i2c clock pin

#define SDA1					10		// PA10	// i2c data pin
#define SCL1					11		// PA11	// i2c clock pin

#define I2C0_SDA_READ		DrvGPIO_GetBit(E_GPA, SDA0)
#define I2C0_SCL_READ		DrvGPIO_GetBit(E_GPA, SCL0)

#define I2C0_SDA_A_READ		DrvGPIO_GetBit(E_GPA, SDAO_A)
#define I2C0_SDA_B_READ		DrvGPIO_GetBit(E_GPE, SDAO_B)

#ifdef _i2C_TEST_
#define I2C0_SDA_A_READ		DrvGPIO_GetBit(E_GPA, SDAO_A_TSET) // test
#define I2C0_SDA_B_READ		DrvGPIO_GetBit(E_GPC, SDAO_B_TEST) // test
#endif

#define I2C0_SDA_HI			DrvGPIO_SetBit(E_GPA, SDA0)
#define I2C0_SDA_LO			DrvGPIO_ClrBit(E_GPA, SDA0)

#define I2C0_SCL_HI			DrvGPIO_SetBit(E_GPA, SCL0)
#define I2C0_SCL_LO			DrvGPIO_ClrBit(E_GPA, SCL0)

#define SCL0_RD_SET			DrvGPIO_Open(E_GPA, SCL0, E_IO_INPUT)
#define SCL0_WR_SET			DrvGPIO_Open(E_GPA, SCL0, E_IO_OUTPUT)

#define SDA0_RD_SET			DrvGPIO_Open(E_GPA, SDA0, E_IO_INPUT)//I2C0_SDA_LO;I2C0_SDA_HI//
#define SDA0_WR_SET			DrvGPIO_Open(E_GPA, SDA0, E_IO_OUTPUT)//I2C0_SDA_HI//

#define I2C1_SDA_READ		DrvGPIO_GetBit(E_GPA, SDA1)
#define I2C1_SCL_READ		DrvGPIO_GetBit(E_GPA, SCL1)

#define I2C1_SDA_HI			DrvGPIO_SetBit(E_GPA, SDA1)
#define I2C1_SDA_LO			DrvGPIO_ClrBit(E_GPA, SDA1)

#define I2C1_SCL_HI			DrvGPIO_SetBit(E_GPA, SCL1)
#define I2C1_SCL_LO			DrvGPIO_ClrBit(E_GPA, SCL1)

#define SDA1_RD_SET			I2C1_SDA_LO;I2C1_SDA_HI//DrvGPIO_Open(E_GPA, SDA1, E_IO_INPUT)
#define SDA1_WR_SET			I2C1_SDA_HI//DrvGPIO_Open(E_GPA, SDA1, E_IO_OUTPUT)

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define I2C_DELAY_P			25
#define I2C_DELAY_0			5//+I2C_DELAY_P
#define I2C_DELAY_1			1+I2C_DELAY_0
#define I2C_DELAY_2			2+I2C_DELAY_0
#define I2C_DELAY_3			3+I2C_DELAY_0
#define I2C_DELAY_4			4+I2C_DELAY_0
#define I2C_DELAY_5			5+I2C_DELAY_0

#define I2C_CHECK_PIN_DUMMY	I2C_DELAY_P

#define I2C_BAUDRATE		400000L

typedef enum 
{
    I2C_PORT0 = 0,
    I2C_PORT1 = 1
} E_I2C_PORT;

#define I2C_STA				0x20
#define I2C_STO				0x10
#define I2C_SI				0x08
#define I2C_AA				0x04

#define NACK				0
#define ACK					1

enum{
	STAT_OK_START		= 0x08,
	STAT_OK_RESTART		= 0x10,
	STAT_OK_SLV_WR		= 0x18,
	STAT_OK_SLV_RD		= 0x40,
	STAT_OK_WR_DATA		= 0x28,
	STAT_OK_RD_DATA_A	= 0x50,
	STAT_OK_RD_DATA_N	= 0x58,

	STAT_NACK_SLV_WR	= 0x20,
	STAT_NACK_WR_DATA	= 0x30,
	STAT_ARBIT_LOST		= 0x38,
	STAT_BUS_ERR		= 0x00,
};

#if I2CMODE == USE_SOFTWAREI2C
#define i2c_restart(p);			i2c_start(p);
#define i2c_PutSlvAddr(p,x);	i2c_Putbyte(p,x);
#endif

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void I2C_Init(void);
//void i2cSetBitrate();

#if I2CMODE == USE_SOFTWAREI2C
void i2cSetSCL(unsigned char port, unsigned char bSet);
void i2cSetSDA(unsigned char port, unsigned char bSet);
void i2cSetSDA_In(unsigned char port);
void i2cSetSDA_Out(unsigned char port);
unsigned char i2cSCL_Read(unsigned char port);
unsigned char i2cSDA_Read(unsigned char port);
void i2cSetSCL_Chk(unsigned char port, unsigned char bSet);
void i2cSetSDA_Chk(unsigned char port, unsigned char bSet);
void i2cSetSCL_Toggle(unsigned char port);
void i2cACK_Write(unsigned char port, unsigned char ackFlag);
unsigned char i2cACK_Read(unsigned char port);
#endif

void i2c_start(unsigned char port);
void i2c_stop(unsigned char port);
#if I2CMODE == USE_HARDWAREI2C
void i2c_restart(unsigned char port);
void i2c_PutSlvAddr(unsigned char port, unsigned char slv_addr);
#endif
void i2c_Putbyte(unsigned char port, unsigned char w_data);
unsigned char i2c_Getbyte(unsigned char port, unsigned char devAddr, unsigned char ackFlag);

void i2c_Write_Addr(unsigned char port, unsigned char devAddr, unsigned char regAddr);
void i2c_Write_Byte(unsigned char port, unsigned char devAddr, unsigned char regAddr, unsigned char data);
void i2c_Write_Bytes(unsigned char port, unsigned char devAddr, unsigned char regAddr, unsigned char *data, unsigned int length);
void i2c_Write_pBytes(unsigned char port, unsigned char devAddr, unsigned char regAddr, const unsigned char *data, unsigned int length);
//void i2c_Write_CnD(unsigned char devAddr, unsigned char regAddr, unsigned char data, unsigned int length);
void i2c_Write_Cn(unsigned char port, unsigned char devAddr, unsigned char regAddr, const unsigned char *data, unsigned int length);
unsigned char i2c_Read_Byte(unsigned char port, unsigned char devAddr, unsigned char regAddr);
void i2c_Read_Bytes(unsigned char port, unsigned char devAddr, unsigned char regAddr, unsigned char *r_data, unsigned int length);

#endif
