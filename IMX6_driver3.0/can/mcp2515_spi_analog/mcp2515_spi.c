
#include <linux/device.h>

#include "mcp2515_spi.h"
#include <linux/gpio.h>
#include <linux/delay.h>
#include <linux/io.h>
#include <linux/of_gpio.h>

#define mcp2515_printk  

#define IMX_GPIO_NR(bank, nr)		(((bank) - 1) * 32 + (nr))

#define GPIO4_06 IMX_GPIO_NR(4, 6) 
#define GPIO4_08 IMX_GPIO_NR(4, 8) 
#define GPIO4_07 IMX_GPIO_NR(4, 7) 
#define GPIO4_09 IMX_GPIO_NR(4, 9) 
#define GPIO4_11 IMX_GPIO_NR(4, 11) 

#define SCK   GPIO4_06
#define MISO  GPIO4_08
#define MOSI  GPIO4_07
#define NSS   GPIO4_09
#define INT   GPIO4_11


/*spi read a byte*/
unsigned char SPI_ReadByte(void)
{

	unsigned char i,rByte=0;
	gpio_direction_output(SCK,0);
	gpio_direction_input(MISO);
	gpio_set_value(SCK,0);//MCP2515_SCK=0;
	for(i=0;i<8;i++)
	{
		gpio_set_value(SCK,1);//MCP2515_SCK=1;
		rByte<<=1;
		rByte|=gpio_get_value(MISO);
		gpio_set_value(SCK,0);//MCP2515_SCK=0;
		return rByte;
	}
}

/*spi send a byte*/
void SPI_SendByte(unsigned char dt)
{
	
	unsigned char i;
	gpio_direction_output(SCK,0);
	for(i=0;i<8;i++)
	{
		
		gpio_set_value(SCK,0);//MCP2515_SCK=0;
		if((dt<<i)&0x80)
		 	gpio_set_value(MOSI,1);
		else
		 	gpio_set_value(MOSI,0);
		 
			gpio_set_value(SCK,1);//MCP2515_SCK=1;
	}
	gpio_set_value(SCK,0);//MCP2515_SCK=0;
}

/*IMX6 send ground data to MCP2515*/
void MCP2515_WriteByte(unsigned char addr,unsigned char dat)
{
	
	gpio_set_value(NSS,0);//MCP2515_CS=0; 		//CS low level
	SPI_SendByte(CAN_WRITE);//write command
	SPI_SendByte(addr); 	//send address
	SPI_SendByte(dat); 	//write data
	gpio_set_value(NSS,1);//MCP2515_CS=1; 		//CS high level

}

/*IMX6 read MCP2515 data*/
unsigned char MCP2515_ReadByte(unsigned char addr)
{
	unsigned char rByte;
	gpio_set_value(NSS,0);//MCP2515_CS=0; 		//CS low level
	SPI_SendByte(CAN_READ); //write command
	SPI_SendByte(addr);  	//send address
	rByte=SPI_ReadByte(); 	//read data
	gpio_set_value(NSS,1);//MCP2515_CS=1;  		//CS high level
	return rByte;	
}

/*software reset MCP2515*/
void MCP2515_Reset(void)
{
	gpio_set_value(NSS,0);//MCP2515_CS=0;
	SPI_SendByte(CAN_RESET);
	gpio_set_value(NSS,1);//MCP2515_CS=1;
}



void CAN_set_var(unsigned char address,unsigned char mask,unsigned char dat)
{
	gpio_set_value(NSS,1);//MCP2515_CS=1;
	udelay(1);
	gpio_set_value(NSS,0);//MCP2515_CS=0;
	SPI_SendByte(0x05);
	SPI_SendByte(address);
	SPI_SendByte(mask);
	SPI_SendByte(dat);
	gpio_set_value(NSS,1);//MCP2515_CS=1;

}



void MCP2515_CAN_INIT(void)
{
	MCP2515_Reset();//send software reset
	mdelay(1);	
	CAN_set_var(0x0f,0xe0,0x80);//setting configuration mode
	/*bound 125000*/
	MCP2515_WriteByte(CNF1,CAN_125Kbps);
	MCP2515_WriteByte(CNF2,0x80|PHSEG1_3TQ|PRSEG_1TQ);
	MCP2515_WriteByte(CNF3,PHSEG2_3TQ);

	MCP2515_WriteByte(0x60,0x60);//close receive all data
	
	/*filtra*/
	MCP2515_WriteByte(0x00,0);
	MCP2515_WriteByte(0x01,0);
	MCP2515_WriteByte(0x02,0);
	MCP2515_WriteByte(0x03,0);
	/*mask*/
	MCP2515_WriteByte(0x20,0);
	MCP2515_WriteByte(0x21,0);
	MCP2515_WriteByte(0x22,0);
	MCP2515_WriteByte(0x23,0);

	MCP2515_WriteByte(0x2b,0x01);//receive data trigger interrput
	//CAN_set_var(0x0f,0xe0,0x40);//setting loop mode
	CAN_set_var(0x0f,0xe0,0x00);//work mode

}
