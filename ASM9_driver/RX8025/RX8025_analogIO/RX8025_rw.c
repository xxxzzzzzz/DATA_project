#include <linux/device.h>

#include <linux/gpio.h>
#include <linux/delay.h>
#include <linux/io.h>
#include<mach/pincontrol.h>
#include <linux/bcd.h>
#include <linux/rtc.h>
#include <linux/string.h>
#include "RX8025_rw.h"

#define SECONDS    0X00
#define MINUTES    0X01
#define HOURS      0x02
#define WEEKDAYs   0x03
#define DAYS       0x04
#define MONTHS     0X05
#define YEARS      0x06
#define CONTROL1   0x0e
#define CONTROL2   0x0f

#define ALARM_D_MIN  0x0b
#define ALARM_D_HOUR 0x0c

void SetPinDir(uint32_t port,uint32_t pin,uint8_t bOut)
{
	if(bOut)
		as3310_writel_gpio((1<<((port%4)*8+pin)),HW_GPIO_DATA_BASE+0x8000 + (port/4)*0x10000  + 4);
	else
		as3310_writel_gpio((1<<((port%4)*8+pin)),HW_GPIO_DATA_BASE+0x8000 + (port/4)*0x10000  + 8);
}


void SetGpio(uint32_t port,uint32_t pin)
{
	as3310_writel_gpio(1<<((port%4)*8+pin),HW_GPIO_DATA_BASE + (port/4)*0x10000+4);
}

void clearGpio(uint32_t port,uint32_t pin)
{
	as3310_writel_gpio(1<<((port%4)*8+pin),HW_GPIO_DATA_BASE + (port/4)*0x10000+8);
}


/*************************************************************************************/
void IIC_Start(struct rx8025_GPIO_definit* plat_gpio_data)
{
	SetPinDir(plat_gpio_data->rx8025_IO_GPIOgroup,plat_gpio_data->rx8025_IO_pin,1);//SDA OUT++     
	SetPinDir(plat_gpio_data->rx8025_SCLK_GPIOgroup,plat_gpio_data->rx8025_SCLK_pin,1);//SCLK OUT++

	SetGpio(plat_gpio_data->rx8025_SCLK_GPIOgroup,plat_gpio_data->rx8025_SCLK_pin); //SCLK=1
	SetGpio(plat_gpio_data->rx8025_IO_GPIOgroup,plat_gpio_data->rx8025_IO_pin); //SDA=1
	udelay(10);
	clearGpio(plat_gpio_data->rx8025_IO_GPIOgroup,plat_gpio_data->rx8025_IO_pin);//SDA=0
	udelay(10);
	clearGpio(plat_gpio_data->rx8025_SCLK_GPIOgroup,plat_gpio_data->rx8025_SCLK_pin);//SCLK=0
	udelay(10);
} 
 
void IIC_Stop(struct rx8025_GPIO_definit* plat_gpio_data)
{
	unsigned char i;
	SetPinDir(plat_gpio_data->rx8025_IO_GPIOgroup,plat_gpio_data->rx8025_IO_pin,1);//SDA OUT++

	clearGpio(plat_gpio_data->rx8025_IO_GPIOgroup,plat_gpio_data->rx8025_IO_pin);//SDA=0
	udelay(10);
	SetGpio(plat_gpio_data->rx8025_SCLK_GPIOgroup,plat_gpio_data->rx8025_SCLK_pin); //SCLK=1
	udelay(10);
	SetGpio(plat_gpio_data->rx8025_IO_GPIOgroup,plat_gpio_data->rx8025_IO_pin); //SDA=1

	for(i=0;i<5;i++)
	{
		udelay(10);
												
	}   	
}
 
u8 IIC_Wait_Ack(struct rx8025_GPIO_definit* plat_gpio_data)
{
	unsigned char ucErrTime=0;
       	SetPinDir(plat_gpio_data->rx8025_IO_GPIOgroup,plat_gpio_data->rx8025_IO_pin,0);//SDA IN++	
	udelay(10);
	udelay(10);

	while(read_GPIO(plat_gpio_data->rx8025_IO_GPIOgroup,plat_gpio_data->rx8025_IO_pin))
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			IIC_Stop(plat_gpio_data);
			return 1;
		}
	}
	clearGpio(plat_gpio_data->rx8025_SCLK_GPIOgroup,plat_gpio_data->rx8025_SCLK_pin);//SCLK=0
	return 0;  
} 
 
void IIC_Ack(struct rx8025_GPIO_definit* plat_gpio_data)
{
		
	SetPinDir(plat_gpio_data->rx8025_IO_GPIOgroup,plat_gpio_data->rx8025_IO_pin,1);//SDA OUT++

	clearGpio(plat_gpio_data->rx8025_IO_GPIOgroup,plat_gpio_data->rx8025_IO_pin);//SDA=0
	udelay(10);
	SetGpio(plat_gpio_data->rx8025_SCLK_GPIOgroup,plat_gpio_data->rx8025_SCLK_pin); //SCLK=1
	udelay(10);
	clearGpio(plat_gpio_data->rx8025_SCLK_GPIOgroup,plat_gpio_data->rx8025_SCLK_pin);//SCLK=0
	udelay(10);
}
	    
void IIC_NAck(struct rx8025_GPIO_definit* plat_gpio_data)
{
	SetPinDir(plat_gpio_data->rx8025_IO_GPIOgroup,plat_gpio_data->rx8025_IO_pin,1);//SDA OUT++

	SetGpio(plat_gpio_data->rx8025_IO_GPIOgroup,plat_gpio_data->rx8025_IO_pin); //SDA=1
	udelay(10);
	SetGpio(plat_gpio_data->rx8025_SCLK_GPIOgroup,plat_gpio_data->rx8025_SCLK_pin); //SCLK=1
	udelay(10);
	clearGpio(plat_gpio_data->rx8025_SCLK_GPIOgroup,plat_gpio_data->rx8025_SCLK_pin);//SCLK=0
	udelay(10);
} 				     
		  
void IIC_Send_Byte(unsigned char txd,struct rx8025_GPIO_definit* plat_gpio_data)
{ 
	unsigned char i;
	SetPinDir(plat_gpio_data->rx8025_IO_GPIOgroup,plat_gpio_data->rx8025_IO_pin,1);//SDA OUT++

	for(i=0;i<8;i++)
	{
		if(txd & 0x80)
			SetGpio(plat_gpio_data->rx8025_IO_GPIOgroup,plat_gpio_data->rx8025_IO_pin); //SDA=1
		else 
			clearGpio(plat_gpio_data->rx8025_IO_GPIOgroup,plat_gpio_data->rx8025_IO_pin);//SDA=0
		txd <<= 1;
		udelay(10);
		SetGpio(plat_gpio_data->rx8025_SCLK_GPIOgroup,plat_gpio_data->rx8025_SCLK_pin); //SCLK=1
		udelay(10);
		clearGpio(plat_gpio_data->rx8025_SCLK_GPIOgroup,plat_gpio_data->rx8025_SCLK_pin);//SCLK=0
	}
	udelay(10);
	SetGpio(plat_gpio_data->rx8025_SCLK_GPIOgroup,plat_gpio_data->rx8025_SCLK_pin); //SCLK=1
	udelay(10);
	clearGpio(plat_gpio_data->rx8025_SCLK_GPIOgroup,plat_gpio_data->rx8025_SCLK_pin);//SCLK=0
} 	    
 
unsigned char IIC_Read_Byte(struct rx8025_GPIO_definit* plat_gpio_data)
{
		unsigned char i,receive=0;
	
		SetPinDir(plat_gpio_data->rx8025_IO_GPIOgroup,plat_gpio_data->rx8025_IO_pin,0);//SDA IN++

		for(i=0;i<8;i++)
		{
			SetGpio(plat_gpio_data->rx8025_SCLK_GPIOgroup,plat_gpio_data->rx8025_SCLK_pin); //SCLK=1
			udelay(10);
			receive <<=1;
			if(read_GPIO(plat_gpio_data->rx8025_IO_GPIOgroup,plat_gpio_data->rx8025_IO_pin)) 
				receive |= 0x01;

				clearGpio(plat_gpio_data->rx8025_SCLK_GPIOgroup,plat_gpio_data->rx8025_SCLK_pin);//SCLK=0
			udelay(10);
		}
		SetPinDir(plat_gpio_data->rx8025_IO_GPIOgroup,plat_gpio_data->rx8025_IO_pin,1);//SDA OUT++
		return receive;

}
 
unsigned char  R8025AC_Read_byte(unsigned char address,struct rx8025_GPIO_definit* plat_gpio_data)
{
	unsigned char ret;
	IIC_Start(plat_gpio_data);
	IIC_Send_Byte(0x64,plat_gpio_data);
	IIC_Send_Byte(address<<4|0x01,plat_gpio_data);
	IIC_Start(plat_gpio_data);
	IIC_Send_Byte(0x65,plat_gpio_data);
										
	ret = IIC_Read_Byte(plat_gpio_data);
	IIC_NAck(plat_gpio_data);
	IIC_Stop(plat_gpio_data);
	return ret;
}
 
void R8025AC_Read(unsigned char sadd, unsigned char *buf, unsigned char len,struct rx8025_GPIO_definit* plat_gpio_data)
{
	unsigned char i;
	IIC_Start(plat_gpio_data);
	IIC_Send_Byte(0x64,plat_gpio_data);
	IIC_Send_Byte(sadd<<4|0x01,plat_gpio_data);
	IIC_Start(plat_gpio_data);
	IIC_Send_Byte(0x65,plat_gpio_data);
	for(i=0;i<len-1;i++)
	{
										
		buf[i] = IIC_Read_Byte(plat_gpio_data);
		IIC_Ack(plat_gpio_data);
	}
	buf[i] = IIC_Read_Byte(plat_gpio_data);
	IIC_NAck(plat_gpio_data);
	IIC_Stop(plat_gpio_data);
}
 
void R8025AC_Write(unsigned char address,unsigned char value,struct rx8025_GPIO_definit* plat_gpio_data)
{
		
	IIC_Start(plat_gpio_data);
	IIC_Send_Byte(0x64,plat_gpio_data);
	IIC_Send_Byte((address<<4),plat_gpio_data);
	IIC_Send_Byte(value,plat_gpio_data);
	IIC_Stop(plat_gpio_data);
}
 
void RX8025_regTime_test(struct rx8025_GPIO_definit  * plat_gpio_data)
{

	unsigned char temp = 0,i;
	unsigned char buf[6],data;

	R8025AC_Write(0x0e,0x20,plat_gpio_data);
	data = R8025AC_Read_byte(0x00,plat_gpio_data);
	printk("read 0x0e = %x\n",data);
	R8025AC_Write(0x00,bin2bcd(30),plat_gpio_data);

	for(i =0 ;i<30;i++)
	{
	//	R8025AC_Read(0x00,buf,6,plat_gpio_data);
		data = R8025AC_Read_byte(0x00,plat_gpio_data);
		printk("sec = %d\n",bcd2bin(data));
		mdelay(500);
	}
		
}




void RX8025_Time_set(struct RX8025_Time *tm,struct rx8025_GPIO_definit* plat_gpio_data)
{
	unsigned char temp = 0,i = 0;
	unsigned char rtc_str[7]={0};
      	
	unsigned char bcd8421[60]={0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0a,0x0b,0x0c,0x0d,0x0e,0x0f,0x10,0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1a,0x1b,0x1c,0x1d,0x1e,0x1f,0x20,0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,0x2a,0x2b,0x2c,0x2d,0x2e,0x2f,0x30,0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x3a,0x3b};

	temp = R8025AC_Read_byte(CONTROL1,plat_gpio_data);


	if((R8025AC_Read_byte(CONTROL1,plat_gpio_data) & 0x20)!=0X20)
	{
		temp = R8025AC_Read_byte(CONTROL1,plat_gpio_data);
		temp |= 1<<5;
		R8025AC_Write(CONTROL1,temp,plat_gpio_data);
	}

	
	rtc_str[0] = bcd8421[tm->sec];
	rtc_str[1] = bcd8421[tm->min];
	rtc_str[2] = bcd8421[tm->hours];
	rtc_str[3] = bcd8421[tm->week];
	rtc_str[4] = bcd8421[tm->day];
	rtc_str[5] = bcd8421[tm->month];
	rtc_str[6] = bcd8421[tm->year];
	

	for(i = 0;i < 7;i++)
	{
		R8025AC_Write(i,bin2bcd(rtc_str[i]),plat_gpio_data);
	}

}




void RX8025_get_Time(struct RX8025_Time *tm,struct rx8025_GPIO_definit* plat_gpio_data)
{
	unsigned char temp = 0,i = 0;
	unsigned char rtc_str[7]={0};

	if((R8025AC_Read_byte(CONTROL1,plat_gpio_data) & 0x20)!=0X20)
	{
		temp = R8025AC_Read_byte(CONTROL1,plat_gpio_data);
		temp |= 1<<5;
		R8025AC_Write(CONTROL1,temp,plat_gpio_data);
	}
	
	temp = R8025AC_Read_byte(CONTROL1,plat_gpio_data);
	printk("CONTROL1 = %x\n",temp);

	for(i=0;i<7;i++)
	{
		rtc_str[i] = R8025AC_Read_byte(i,plat_gpio_data);
	}

	tm->sec = (unsigned int)bcd2bin(rtc_str[0]);
	tm->min = (unsigned int)bcd2bin(rtc_str[1]);
	tm->hours = (unsigned int)bcd2bin(rtc_str[2]);
	tm->week = (unsigned int)bcd2bin(rtc_str[3]);
	tm->day = (unsigned int)bcd2bin(rtc_str[4]);
	tm->month = (unsigned int)bcd2bin(rtc_str[5]);
	tm->year = (unsigned int)bcd2bin(rtc_str[6]);
	
}


/*
 *0x00 INTA pin not Out
 *0x01 INTA pin Out continue Low level
 *0x02 INTA pin Out duty cycle 50% 2hz pulse
 *0x03 INTA pin Out duty cycle 50% 1hz pulse
 *0x04 INTA pin Out Low level ,1 sec Out at a time,But Out after Low use software set Control2(0x0f) reg CTFG set 1
 *     in this way 1 sec Time out  INTA again Out Low level 
 *0x05 INTA pin 1 min Out at a time Low level ,min time chip default judge min reg ,Out after CTFG set 1  
 *0x06 INTA pin 1 hours Out at a time Low level,Out after CTFG set 1 
 *0x07 INTA pin 1 Month Out at a time Low level,Out after CTFG set 1
 */

void set_INTA_out_Mode(unsigned char CT,struct rx8025_GPIO_definit* plat_gpio_data)
{
	unsigned char temp = 0x20;
	
	temp = R8025AC_Read_byte(0x0e,plat_gpio_data);
	temp &= 0xf8;
	temp |= CT;
	R8025AC_Write(CONTROL1,temp,plat_gpio_data);
}


/*software INTA Out leve Time
 * *tm : value set start time sec min hours week day month year 
 * setmin : set time out INTA Out level minture
 * sethour: set time out INTA Out level hours
 * */
void RX8025_INTA_OutIrq_set(struct RX8025_Time *tm,unsigned char setmin,unsigned char sethour,struct rx8025_GPIO_definit* plat_gpio_data)
{
	unsigned char temp = 0x20,minreg=0,hourreg=0;
	
	temp = R8025AC_Read_byte(CONTROL1,plat_gpio_data);

	R8025AC_Write(ALARM_D_MIN,bin2bcd(setmin),plat_gpio_data);//set time out min
	R8025AC_Write(ALARM_D_HOUR,bin2bcd(sethour),plat_gpio_data);//set time out hours

	temp |= 0x40;
	R8025AC_Write(CONTROL1,temp,plat_gpio_data);

	printk("set Alarm D time value CONTROL1 = 0x%x\n",temp);
	
	RX8025_Time_set(tm,plat_gpio_data);

	minreg = R8025AC_Read_byte(ALARM_D_MIN,plat_gpio_data);
	hourreg = R8025AC_Read_byte(ALARM_D_HOUR,plat_gpio_data);
	printk("minreg = 0x%x  hourreg = 0x%x \n",minreg,hourreg);
}

void RX8025_INTA_REG_CONTROL2_get_set_CTFG(struct rx8025_GPIO_definit* plat_gpio_data)
{
	unsigned char temp = 0,CTFG;

	temp = R8025AC_Read_byte(CONTROL2,plat_gpio_data);
	printk("0x0f = 0x%x\n",temp);
	
	if(((R8025AC_Read_byte(CONTROL2,plat_gpio_data)&0x11)==0x11)||
		((R8025AC_Read_byte(CONTROL2,plat_gpio_data)&0X01)==0X01))
	{
		CTFG = R8025AC_Read_byte(CONTROL2,plat_gpio_data);
		CTFG &= 0xf8;
		R8025AC_Write(CONTROL2,CTFG,plat_gpio_data);

		temp = R8025AC_Read_byte(CONTROL2,plat_gpio_data);
		printk("reset 0x0f = 0x%x\n",temp);
	}

}
