#include <linux/device.h>
#include <linux/gpio.h>
#include <linux/delay.h>
#include <linux/io.h>
#include<mach/pincontrol.h>
//#define DEBUG

unsigned long Read_HX711(int clk_GPIOgroup,int clk_pin,int data_GPIOgroup,int data_pin){
	
	unsigned long val=0,z=0;
	unsigned char i=0;
	write_GPIO(data_GPIOgroup,data_pin,1); //dat = 1
 	 /*set dat dir input*/
 	write_GPIO(clk_GPIOgroup,clk_pin,0);//SCK = 0
 	while(read_GPIO(data_GPIOgroup,data_pin)!=0)
	{
		z++;
		if(z>60000)
		{
#ifdef DEBUG
			printk("hx711 disconnect\n");
#endif
			z = 0;
			return 0xfffffff;
		}
	}
	z = 0;
 	udelay(1);
 	for(i=0;i<24;i++){
	
 	write_GPIO(clk_GPIOgroup,clk_pin,1);//SCK = 1
 	val=val<<1;
 	udelay(1);
 	write_GPIO(clk_GPIOgroup,clk_pin,0);//SCK = 0
 	if(read_GPIO(data_GPIOgroup,data_pin)!=0)
 		val++;
 		udelay(1);
  	}
	write_GPIO(clk_GPIOgroup,clk_pin,1);//SCK = 1
 	val=val^0x800000;
 	udelay(1);
	write_GPIO(clk_GPIOgroup,clk_pin,0);//SCK = 0
 	udelay(1);
	return val;
}

//////////////////get_pressure/////
unsigned long get_weight(int clk_GPIOgroup,int clk_pin,int data_GPIOgroup,int data_pin){

	unsigned int count=0;
	unsigned long sum=0;
	unsigned long zhongliang;
	for(count=0;count<10;count++){
	
	zhongliang=Read_HX711(clk_GPIOgroup,clk_pin,data_GPIOgroup,data_pin);
#ifdef DEBUG
	printk("kernel zhongliang = %ld hex = %x\n",zhongliang,zhongliang);
#endif
	if(zhongliang == 0xfffffff)
		return 0xfffffff;
	zhongliang=(~zhongliang);
	zhongliang=zhongliang>>8;
	sum+=zhongliang;
	}
	zhongliang=sum/10;
	return zhongliang;

}

