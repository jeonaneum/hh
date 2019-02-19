#include "main.h"
#include "i2c.h"
#include "SOH701.h"
#include "common.h"
#include <stdio.h>

// OSD Control -----------------------------------------------
void i2c_Write (u08 data)
{
	i2c_Putbyte(I2C_PORT0, data);
	
//	printf("data : %x \n", data);
}

void Osd_Locate(u08 row, u08 col)
{
	delay(I2C_DELAY_0);
	i2c_start(I2C_PORT0);
	i2c_PutSlvAddr(I2C_PORT0, SOH7_ADDR);
	i2c_Write(SOH7_STX);
	i2c_Write(FOSD_LOCATE_POS);
	i2c_Write(row);
	i2c_Write(col);
	i2c_Write(FOSD_ETX);
	i2c_stop(I2C_PORT0);
}

void Osd_PutStr(u08 page, u08 *str)
{
	delay(I2C_DELAY_0);
	if(*str == 0) return;
	i2c_start(I2C_PORT0);
	i2c_PutSlvAddr(I2C_PORT0, SOH7_ADDR);	
	i2c_Write(SOH7_STX);
	i2c_Write(FOSD_CHAR_WRITE);
	i2c_Write(page);
	while(*str)
	{
		i2c_Write(*str);
		str++;
	}
	i2c_Write(FOSD_ETX);
	i2c_stop(I2C_PORT0);
}

void Osd_PutChar(u08 page, u08 *str, u08 length)
{
	u08 i;
	if(length == 0) return;
	i2c_start(I2C_PORT0);
	i2c_PutSlvAddr(I2C_PORT0, SOH7_ADDR);	
	i2c_Write(SOH7_STX);
	i2c_Write(FOSD_CHAR_WRITE);
	i2c_Write(page);
	for(i=0; i<length; i++)
	{
		i2c_Write(*str);
		str++;
	}
	i2c_Write(FOSD_ETX);
	i2c_stop(I2C_PORT0);
}

void Osd_Erase(u08 del_row)
{
	u08 i;
	
	for(i=1; i<=del_row; i++)
	{
		Osd_Locate(i, 1);
		Osd_PutStr(PAGE_ENGLISH, (u08*)"                        ");
	}
}
void Osd_Clear(void)
{
	i2c_start(I2C_PORT0);
	i2c_PutSlvAddr(I2C_PORT0, SOH7_ADDR);	
	i2c_Write(SOH7_STX);
	i2c_Write(FOSD_CLEAR_ALL);
	i2c_Write(FOSD_ETX);
	i2c_stop(I2C_PORT0);
}


