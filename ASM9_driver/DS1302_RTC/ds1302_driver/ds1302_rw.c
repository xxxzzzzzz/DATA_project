
#include <linux/device.h>

#include <linux/gpio.h>
#include <linux/delay.h>
#include <linux/io.h>
#include<mach/pincontrol.h>
#include "ds1302_rw.h"

#define DEBUG

/*write ds1302 register 1  byte */
void DS1302_writeByte(unsigned char data,struct Cdata_DS1302_GPIO_definit* plat_gpio_data){
	
	unsigned char i,temp,ds1302_data;

	temp=data;
	for(i=8;i>0;i--){
	ds1302_data=temp&0x01;
	write_GPIO(plat_gpio_data->ds1302_IO_GPIOgroup,plat_gpio_data->ds1302_IO_pin,ds1302_data); //IO 
	write_GPIO(plat_gpio_data->ds1302_SCLK_GPIOgroup,plat_gpio_data->ds1302_SCLK_pin,1); //SCLK=1
	udelay(1);
	write_GPIO(plat_gpio_data->ds1302_SCLK_GPIOgroup,plat_gpio_data->ds1302_SCLK_pin,0); //SCLK=0
	udelay(1);
	temp=temp>>1;
	}
}




/*read ds1302 register 1  byte */
unsigned char DS1302_ReadByte(struct Cdata_DS1302_GPIO_definit* plat_gpio_data){
	
	unsigned char i,temp;
	write_GPIO(plat_gpio_data->ds1302_SCLK_GPIOgroup,plat_gpio_data->ds1302_SCLK_pin,0); //SCLK=0
	//res=gpio_direction_input(dat);//IO set input
	temp=0;
	for(i=8;i>0;i--){
	temp=temp>>1;
	if(read_GPIO(plat_gpio_data->ds1302_IO_GPIOgroup,plat_gpio_data->ds1302_IO_pin)){
		temp=temp+0x80;
	}

	write_GPIO(plat_gpio_data->ds1302_SCLK_GPIOgroup,plat_gpio_data->ds1302_SCLK_pin,1); //SCLK=1
	udelay(1);
	write_GPIO(plat_gpio_data->ds1302_SCLK_GPIOgroup,plat_gpio_data->ds1302_SCLK_pin,0); //SCLK=0
	udelay(1);
	}
	return temp;
}


/*write DS1302 addr the data*/
void write_DS1302(unsigned char addr,unsigned char DS_dat,struct Cdata_DS1302_GPIO_definit* plat_gpio_data){


	write_GPIO(plat_gpio_data->ds1302_CE_GPIOgroup,plat_gpio_data->ds1302_CE_pin,0);//CE = 0
	write_GPIO(plat_gpio_data->ds1302_SCLK_GPIOgroup,plat_gpio_data->ds1302_SCLK_pin,0); //SCLK=0
	write_GPIO(plat_gpio_data->ds1302_IO_GPIOgroup,plat_gpio_data->ds1302_IO_pin,0); //IO =0
	udelay(1);
	write_GPIO(plat_gpio_data->ds1302_CE_GPIOgroup,plat_gpio_data->ds1302_CE_pin,1);//CE = 1
	udelay(4);
	DS1302_writeByte(addr,plat_gpio_data);
	DS1302_writeByte(DS_dat,plat_gpio_data);

	write_GPIO(plat_gpio_data->ds1302_SCLK_GPIOgroup,plat_gpio_data->ds1302_SCLK_pin,1); //SCLK=1
	udelay(1);
	write_GPIO(plat_gpio_data->ds1302_CE_GPIOgroup,plat_gpio_data->ds1302_CE_pin,0);//CE = 0
}




/*read DS1302 data*/

unsigned char read_DS1302(unsigned char addr,struct Cdata_DS1302_GPIO_definit* plat_gpio_data){
	
	unsigned char temp;
	write_GPIO(plat_gpio_data->ds1302_CE_GPIOgroup,plat_gpio_data->ds1302_CE_pin,0);//CE = 0
	write_GPIO(plat_gpio_data->ds1302_SCLK_GPIOgroup,plat_gpio_data->ds1302_SCLK_pin,0);//SCLK = 0
	write_GPIO(plat_gpio_data->ds1302_IO_GPIOgroup,plat_gpio_data->ds1302_IO_pin,0);//dat = 0
	udelay(1);
	write_GPIO(plat_gpio_data->ds1302_CE_GPIOgroup,plat_gpio_data->ds1302_CE_pin,1);//CE = 1
	udelay(4);
	DS1302_writeByte(addr,plat_gpio_data);
	temp=DS1302_ReadByte(plat_gpio_data);
	write_GPIO(plat_gpio_data->ds1302_SCLK_GPIOgroup,plat_gpio_data->ds1302_SCLK_pin,1);//SCLK = 1
	udelay(1);
	write_GPIO(plat_gpio_data->ds1302_CE_GPIOgroup,plat_gpio_data->ds1302_CE_pin,0);//CE = 0
	return temp;
}


void init_ds1302(unsigned char sec, unsigned char min, unsigned char hour,unsigned char day,
unsigned char month,unsigned char year,struct Cdata_DS1302_GPIO_definit* plat_gpio_data){
	
#ifdef DEBUG
	printk("init ds1302_IO_GPIOgroup = %d\n",plat_gpio_data->ds1302_IO_GPIOgroup);
	printk("init ds1302_IO_pin = %d\n",plat_gpio_data->ds1302_IO_pin);
	printk("init ds1302_SCLK_GPIOgroup = %d\n",plat_gpio_data->ds1302_SCLK_GPIOgroup);
	printk("init ds1302_SCLK_pin = %d\n",plat_gpio_data->ds1302_SCLK_pin);
	printk("init ds1302_CE_GPIOgroup = %d\n",plat_gpio_data->ds1302_CE_GPIOgroup);
	printk("init ds1302_CE_pin = %d\n",plat_gpio_data->ds1302_CE_pin);
#endif

	write_DS1302(WRITE_PROTECT,0x00,plat_gpio_data);//close write baohu
	write_DS1302(charger,0xa2, plat_gpio_data);      //charger size
	write_DS1302(WRITE_SECOND,sec, plat_gpio_data); //second
	write_DS1302(WRITE_MINUTE,min, plat_gpio_data); //minute
	write_DS1302(WRITE_HOUR,hour, plat_gpio_data);   //hours
	write_DS1302(WRITE_DAY,day, plat_gpio_data);    //day
	write_DS1302(WRITE_MONTH,month, plat_gpio_data);  //month
	write_DS1302(WRITE_YEARS,year, plat_gpio_data);  //years

	write_DS1302(WRITE_PROTECT,0x80,plat_gpio_data);//open write baohu


}



	
