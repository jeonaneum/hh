#include <stdio.h>

#include "NUC1xx.h"
#include "DrvSYS.h"
#include "DrvGPIO.h"
#include "main.h"
#include "common.h"
#include "global.h"
#include "i2c.h"
#include "uart.h"


//! Initialize I2C communication
void I2C_Init(void)
{
#if I2CMODE == USE_HARDWAREI2C
	unsigned int divider;	 
#endif

#if I2CMODE == USE_SOFTWAREI2C
#ifdef _I2C0_ENABLE_
	DrvGPIO_Open(E_GPA, SDA0, E_IO_OPENDRAIN);
	DrvGPIO_Open(E_GPA, SCL0, E_IO_OPENDRAIN);		//Configure I2C0 pins open-drain
	I2C0_SCL_HI;									// set I/O state
	I2C0_SDA_HI;									// set I/O state
#endif
#ifdef _I2C1_ENABLE_
	DrvGPIO_Open(E_GPA, SDA1, E_IO_OPENDRAIN);
	DrvGPIO_Open(E_GPA, SCL1, E_IO_OPENDRAIN);		//Configure I2C0 pins open-drain
	I2C1_SCL_HI;									// set I/O state
	I2C1_SDA_HI;									// set I/O state
#endif

#elif I2CMODE == USE_HARDWAREI2C
	divider = (unsigned int)(((SystemCoreClock*10)/(I2C_BAUDRATE*4)+5)/10-1);	// Compute proper divider for I2C clock
	if(divider < 4)	divider = 4;

#ifdef _I2C0_ENABLE_
	DrvGPIO_InitFunction(E_FUNC_I2C0);
	SYSCLK->APBCLK.I2C0_EN = 1;
	SYS->IPRSTC2.I2C0_RST = 1;
	SYS->IPRSTC2.I2C0_RST = 0;  
	I2C0->I2CON.ENS1 = 1;
	I2C0->I2CLK = divider;
#endif
#ifdef _I2C1_ENABLE_

	DrvGPIO_InitFunction(E_FUNC_I2C1);
	SYSCLK->APBCLK.I2C1_EN = 1;
	SYS->IPRSTC2.I2C1_RST = 1;
	SYS->IPRSTC2.I2C1_RST = 0;
	I2C1->I2CON.ENS1 = 1;
	I2C1->I2CLK = divider;
#endif

#endif
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
#if I2CMODE == USE_SOFTWAREI2C

void i2cSetSCL(unsigned char port, unsigned char bSet)
{
	if(port==I2C_PORT0){ (bSet)?I2C0_SCL_HI:I2C0_SCL_LO; }
	if(port==I2C_PORT1){ (bSet)?I2C1_SCL_HI:I2C1_SCL_LO; }
}
void i2cSetSDA(unsigned char port, unsigned char bSet)
{
	if(port==I2C_PORT0){ (bSet)?I2C0_SDA_HI:I2C0_SDA_LO; }
	if(port==I2C_PORT1){ (bSet)?I2C1_SDA_HI:I2C1_SDA_LO; }
}
void i2cSetSDA_In(unsigned char port)
{
	if(port==I2C_PORT0){ SDA0_RD_SET; }
	if(port==I2C_PORT1){ SDA1_RD_SET; }
}
void i2cSetSDA_Out(unsigned char port)
{
	if(port==I2C_PORT0){ SDA0_WR_SET; }
	if(port==I2C_PORT1){ SDA1_WR_SET; }
}
unsigned char i2cSCL_Read(unsigned char port)
{
	if(port==I2C_PORT0)	return I2C0_SCL_READ;
	if(port==I2C_PORT1)	return I2C1_SCL_READ;
	return 0;
}
unsigned char i2cSDA_Read(unsigned char port)
{
	if(port==I2C_PORT0)	return I2C0_SDA_READ;
	if(port==I2C_PORT1)	return I2C1_SDA_READ;
	return 0;
}

void i2cSetSCL_Chk(unsigned char port, unsigned char bSet)
{
	unsigned char ucDummy = I2C_CHECK_PIN_DUMMY;			// loop dummy

	i2cSetSCL(port,bSet);									// set SCL pin
	bSet ^= 1;		
	while((i2cSCL_Read(port) == bSet) && (ucDummy--)) ;		// check SCL pull high or low
}

void i2cSetSDA_Chk(unsigned char port, unsigned char bSet)
{
	unsigned char ucDummy = I2C_CHECK_PIN_DUMMY;			// loop dummy

	i2cSetSDA(port,bSet);									// set SDA pin
	bSet ^= 1;		
	while((i2cSDA_Read(port) == bSet) && (ucDummy--)) ;		// check SCL pull high or low
}

void i2cSetSCL_Toggle(unsigned char port)
{
	delay(I2C_DELAY_0);
	i2cSetSCL_Chk(port,HIGH);		//SCL = 1;
	delay(I2C_DELAY_4);
	i2cSetSCL_Chk(port,LOW);		//SCL = 0;
	delay(I2C_DELAY_0);
}

void i2c_start(unsigned char port)
{
	
//	i2cSetSCL_Chk(port, LOW);
//	delay(I2C_DELAY_3);
	
	i2cSetSCL_Chk(port,HIGH);	delay(I2C_DELAY_0);		//SCL = 1;
	i2cSetSDA_Chk(port,HIGH);	delay(I2C_DELAY_4);		//SDA = 1;
	i2cSetSDA_Chk(port,LOW);	delay(I2C_DELAY_0);		//SDA = 0;
	i2cSetSCL_Chk(port,LOW);	delay(I2C_DELAY_0);		//SCL = 0;
	delay(I2C_DELAY_0);
}
void i2cACK_Write(unsigned char port, unsigned char ackFlag)
{
	i2cSetSDA_Out(port);			//SDA = 1; //... SDA output mode
	ackFlag ^= 1;
	i2cSetSDA_Chk(port,ackFlag);	//set ACK or NACK
	i2cSetSCL_Toggle(port);
}
unsigned char i2cACK_Read(unsigned char port)
{
	unsigned char ack;

	delay(I2C_DELAY_0);
	i2cSetSDA_In(port);				//SDA = 1; //... SDA input mode
	delay(I2C_DELAY_0);
	i2cSetSCL_Chk(port,HIGH);		//SCL = 1;
	ack = i2cSDA_Read(port);
	delay(I2C_DELAY_4);
	i2cSetSCL_Chk(port,LOW);		//SCL = 0;
	delay(I2C_DELAY_0);
	i2cSetSDA_Out(port);			//SDA = 1; //... SDA output mode

	return ack;
}
void i2c_stop(unsigned char port)
{
	i2cSetSDA_Chk(port,LOW);	delay(I2C_DELAY_0);		//SDA = 0;
	i2cSetSCL_Chk(port,HIGH);	delay(I2C_DELAY_0);		//SCL = 1;
	i2cSetSDA_Chk(port,HIGH);	delay(I2C_DELAY_0);		//SDA = 1;
	delay(I2C_DELAY_0);
	i2cSetSCL_Chk(port,HIGH);
}

void i2c_Putbyte(unsigned char port, unsigned char w_data)
{
	unsigned char i;
	
//	i2cSetSCL_Chk(port, LOW);
//	delay(I2C_DELAY_3);

	for(i=0; i<8; i++){
		(w_data&(0x80>>i))?(i2cSetSDA_Chk(port,HIGH)):(i2cSetSDA_Chk(port,LOW));
		i2cSetSCL_Toggle(port);
	}

	i = i2cACK_Read(port);			//... read ACK
//	if(video_signal == CVBS)
		delay(75);
//	printf("ack : %d \n", i);
}

unsigned char i2c_Getbyte(unsigned char port, unsigned char devAddr,  unsigned char ackFlag)
{
	unsigned char i,rdata = 0;
		
	i2cSetSDA_In(port);				//SDA = 1; //... SDA input mode

	for(i=0; i<8; i++){
		delay(I2C_DELAY_4);
		i2cSetSCL_Chk(port,HIGH);	//SCL = 1;
		if(i2cSDA_Read(port))	rdata |= (0x80>>i);
		delay(I2C_DELAY_0);
		i2cSetSCL_Chk(port,LOW);	//SCL = 0;
		delay(I2C_DELAY_4);
		delay(I2C_DELAY_4);
	}

	i2cACK_Write(port,ackFlag);		//set ACK or NACK

	return rdata;					// return received byte
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
#elif I2CMODE == USE_HARDWAREI2C
/*
// I2C 통신 속도 설정 (100KHz for slow speed, 400KHz for high speed)
void i2cSetBitrate()
{
	//Set I2C0 clock rate
	//Data Baud Rate of I2C = APBCLK/(4x(I2CLK[7:0]+1))
	//I2CLK = PCLK/(4xI2C baud) - 1 = 50,000,000/(4x200,000) - 1 = 61
	I2CLK = 60;					
}
*/
////////////////////////////////////////////////////////////////////////////////////////////////////////////
// I2C Master Control mode
////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef _DEBUG_
void i2c_stat(unsigned char port, unsigned char stat_ok, unsigned char w_data)	//Check status value
{
	unsigned char read_stat = 0;

	if(port==I2C_PORT0) read_stat = I2C0->I2CSTATUS;
	if(port==I2C_PORT1)	read_stat = I2C1->I2CSTATUS;

	if(read_stat != stat_ok)
	{
		printf("I2C%d ERROR[%02x]:",port,read_stat);

		switch(stat_ok)
		{
			case STAT_OK_START:		printf("Start");	break;
			case STAT_OK_RESTART:	printf("Re-Start");	break;

			case STAT_OK_SLV_WR:
			case STAT_OK_SLV_RD:	printf("Write_Slv 0x%02x",w_data);	break;

			case STAT_OK_WR_DATA:	printf("Write_Data 0x%02x",w_data);	break;

			case STAT_OK_RD_DATA_A:
			case STAT_OK_RD_DATA_N:	printf("Read_data");	break;
		}
		printf("\n");

		//i2c_stop(port);
	}
}
#endif

void i2c_ctrl(unsigned char port, uint8_t start, uint8_t stop, uint8_t intFlag, uint8_t ack)
{
	unsigned int Reg = 0;
		
	if(start)	Reg |= I2C_STA;
	if(stop)	Reg |= I2C_STO;
	if(intFlag)	Reg |= I2C_SI;
	if(ack)		Reg |= I2C_AA;

	if(port==I2C_PORT0){
		*((__IO unsigned int *)&I2C0->I2CON) = (*((__IO unsigned int *)&I2C0->I2CON) & ~0x3C) | Reg;
		if(stop==0)	while(I2C0->I2CON.SI == 0);			//Check SI set or not
		else		while(I2C0->I2CON.STO);				//To ensure STOP condition is generated, polling STO flag
	}
	if(port==I2C_PORT1){
		*((__IO unsigned int *)&I2C1->I2CON) = (*((__IO unsigned int *)&I2C1->I2CON) & ~0x3C) | Reg;
		if(stop==0)	while(I2C1->I2CON.SI == 0);			//Check SI set or not
		else		while(I2C1->I2CON.STO);				//To ensure STOP condition is generated, polling STO flag
	}
/*
	if(port==I2C_PORT0) {
		*((__IO unsigned int *)&I2C0->I2CON) = (*((__IO unsigned int *)&I2C0->I2CON) & ~0x3C) | Reg;
		if(stop==0){
			while(I2C0->I2CON.SI == 0){			// Check SI set or not
				i++;
				if(i == 200){
					#ifdef _DEBUG_
					printf("I2C0 ERROR[%02x]:SI\n", Reg);
					#endif
					break;
				}
			}
		}
		else{
			while(I2C0->I2CON.STO){				// To ensure STOP condition is generated, polling STO flag
				i++;
				if(i == 200){
					#ifdef _DEBUG_
					printf("I2C0 ERROR[%02x]:STO\n", Reg);
					#endif
					break;
				}
			}
		}
	}
	if(port==I2C_PORT1) {
		*((__IO unsigned int *)&I2C1->I2CON) = (*((__IO unsigned int *)&I2C1->I2CON) & ~0x3C) | Reg;
		if(stop==0){
			while(I2C1->I2CON.SI == 0){			// Check SI set or not
				i++;
				if(i == 200){
					#ifdef _DEBUG_
					printf("I2C1 ERROR[%02x]:SI\n", Reg);
					#endif
					break;
				}
			}
		}
		else{
			while(I2C1->I2CON.STO){				// To ensure STOP condition is generated, polling STO flag
				i++;
				if(i == 200){
					#ifdef _DEBUG_
					printf("I2C1 ERROR[%02x]:STO\n", Reg);
					#endif
					break;
				}
			}
		}
	}
*/
}

void i2c_start(unsigned char port)
{
	i2c_ctrl(port, 1, 0, 1, 0);
	
	#ifdef _DEBUG_
	i2c_stat(port, STAT_OK_START, 0);
	#endif
}

void i2c_restart(unsigned char port)			// Repeated START
{
	i2c_ctrl(port, 1, 0, 1, 0);

	#ifdef _DEBUG_
	i2c_stat(port, STAT_OK_RESTART, 0);
	#endif
}

void i2c_stop(unsigned char port)
{
	i2c_ctrl(port, 0, 1, 1, 0);
}

void i2c_PutSlvAddr(unsigned char port, unsigned char slv_addr)
{
	if(port==I2C_PORT0) I2C0->I2CDAT = slv_addr;
	if(port==I2C_PORT1)	I2C1->I2CDAT = slv_addr;
	i2c_ctrl(port, 0, 0, 1, 0);

	#ifdef _DEBUG_
	i2c_stat(port, (slv_addr&0x01)?STAT_OK_SLV_RD:STAT_OK_SLV_WR, slv_addr);
	#endif
}

void i2c_Putbyte(unsigned char port, unsigned char w_data)
{
	if(port==I2C_PORT0) I2C0->I2CDAT = w_data;
	if(port==I2C_PORT1)	I2C1->I2CDAT = w_data;
	i2c_ctrl(port, 0, 0, 1, 0);

	#ifdef _DEBUG_
	i2c_stat(port, STAT_OK_WR_DATA, w_data);
	#endif
}

unsigned char i2c_Getbyte(unsigned char port, unsigned char ackFlag)
{
	unsigned char rdata = 0;

	i2c_ctrl(port, 0, 0, 1, ackFlag);

	#ifdef _DEBUG_
	i2c_stat(port, (ackFlag==NACK)?STAT_OK_RD_DATA_N:STAT_OK_RD_DATA_A, 0);
	#endif

	if(port==I2C_PORT0) rdata = I2C0->I2CDAT;
	if(port==I2C_PORT1)	rdata = I2C1->I2CDAT;

	return rdata;
}
#endif
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//	I2C R/W public functions
//-------------------------------------------------------------------------------------------------
void i2c_Write_Addr(unsigned char port, unsigned char devAddr, unsigned char regAddr)
{
	i2c_start(port);	  			// do start transition
	i2c_PutSlvAddr(port,devAddr);	// send DEVICE address
	i2c_Putbyte(port,regAddr);		// and the subaddress
}

//! Send a byte sequence on the I2C bus
void i2c_Write_Byte(unsigned char port, unsigned char devAddr, unsigned char regAddr, unsigned char data)
{
	i2c_Write_Addr(port,devAddr,regAddr);
	i2c_Putbyte(port,data);			// send data
	i2c_stop(port);					// send STOP transition
}

void i2c_Write_Bytes(unsigned char port, unsigned char devAddr, unsigned char regAddr, unsigned char *data, unsigned int length)
{
	unsigned char i=0;

	i2c_Write_Addr(port,devAddr,regAddr);
	while(length--)	i2c_Putbyte(port,data[i++]);
	i2c_stop(port);
}

void i2c_Write_pBytes(unsigned char port, unsigned char devAddr, unsigned char regAddr, const unsigned char *data, unsigned int length)
{
	unsigned char i=0,buff=0;

	i2c_Write_Addr(port,devAddr,regAddr);
	while(length--){
		buff = data[i++];
		i2c_Putbyte(port,buff);
	}
	i2c_stop(port);
}
/*
void i2c_Write_CnD(unsigned char devAddr, unsigned char regAddr, unsigned char data, unsigned int length)
{
	i2c_Write_Addr(devAddr,regAddr);
	while(length--){
		i2c_Putbyte(data);
		TW8816_W_DELAY;
	}
	i2c_stop();
}
*/
void i2c_Write_Cn(unsigned char port, unsigned char devAddr, unsigned char regAddr, const unsigned char *data, unsigned int length)
{
	unsigned char i=0;

	i2c_Write_Addr(port,devAddr,regAddr);
	while(length--){
		i2c_Putbyte(port,data[i++]);
	}
	i2c_stop(port);
}

unsigned char i2c_Read_Byte(unsigned char port, unsigned char devAddr, unsigned char regAddr)
{
	unsigned char r_data;

	i2c_Write_Addr(port,devAddr,regAddr);
	i2c_restart(port);	  				// do a repeated START
	i2c_PutSlvAddr(port,devAddr|0x01);	// resend DEVICE, with READ bit set
	r_data = i2c_Getbyte(port, devAddr, NACK);	// receive data bytes
	i2c_stop(port);						// send STOP transition

	return r_data;
}

void i2c_Read_Bytes(unsigned char port, unsigned char devAddr, unsigned char regAddr, unsigned char *r_data, unsigned int length)
{
	unsigned char i=0;

	i2c_Write_Addr(port,devAddr,regAddr);
	i2c_restart(port);      			// do a repeated START
	i2c_PutSlvAddr(port,devAddr|0x01);	// resend DEVICE, with READ bit set
	length--;
	for(i=0; i<length; i++){
		r_data[i] = i2c_Getbyte(port, devAddr, ACK);// receive data bytes
	}
	r_data[i] = i2c_Getbyte(port,devAddr, NACK);
	i2c_stop(port);						// send STOP transition
}
