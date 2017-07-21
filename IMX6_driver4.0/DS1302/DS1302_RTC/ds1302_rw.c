
#include <linux/device.h>

#include <linux/gpio.h>
#include <linux/delay.h>
#include <linux/io.h>
#include <linux/of_gpio.h>
#include "ds1302_rw.h"
#define IMX_GPIO_NR(bank, nr)		(((bank) - 1) * 32 + (nr))

#define GPIO3_18 IMX_GPIO_NR(3, 18) 
#define GPIO3_16 IMX_GPIO_NR(3, 16) 
#define GPIO3_23 IMX_GPIO_NR(3, 23) 

#define RST   GPIO3_18
#define sclk  GPIO3_16 
#define dat   GPIO3_23 
static int res;



/*write ds1302 register 1  byte */
void DS1302_writeByte(unsigned char data){
	
unsigned char i,temp,ds1302_data;

temp=data;
for(i=8;i>0;i--){
ds1302_data=temp&0x01;
gpio_set_value(dat,ds1302_data);
gpio_set_value(sclk,1);//sclk=1
udelay(1);
gpio_set_value(sclk,0);//sclk=1
udelay(1);
temp=temp>>1;
}


}


/*read ds1302 register 1  byte */
unsigned char DS1302_ReadByte(void){
	
unsigned char i,temp;
res=gpio_direction_output(sclk,0);
res=gpio_direction_input(dat);
temp=0;
for(i=8;i>0;i--){
temp=temp>>1;
if(gpio_get_value(dat)){
	temp=temp+0x80;
   }


gpio_set_value(sclk,1);//sclk=1
udelay(1);
gpio_set_value(sclk,0);//sclk=1
udelay(1);

}

return temp;

}


/*write DS1302 addr the data*/
void write_DS1302(unsigned char addr,unsigned char DS_dat){


res=gpio_direction_output(RST,0);
res=gpio_direction_output(sclk,0);
res=gpio_direction_output(dat,0);
udelay(1);
gpio_set_value(RST,1);

udelay(4);
DS1302_writeByte(addr);
DS1302_writeByte(DS_dat);

gpio_set_value(sclk,1);//sclk=1
udelay(1);
gpio_set_value(RST,0);//reset=0
}




/*read DS1302 data*/

unsigned char read_DS1302(unsigned char addr){
	
unsigned char temp;
res=gpio_direction_output(RST,0);
res=gpio_direction_output(sclk,0);
res=gpio_direction_output(dat,0);
udelay(1);
gpio_set_value(RST,1);//reset=0
udelay(4);
DS1302_writeByte(addr);
temp=DS1302_ReadByte();
gpio_set_value(sclk,1);//sclk=1
udelay(1);
gpio_set_value(RST,0);//reset=0

return temp;
}


void init_ds1302(unsigned char sec, unsigned char min, unsigned char hour,unsigned char day,
unsigned char month,unsigned char year){
	

write_DS1302(WRITE_PROTECT,0x00);//close write baohu
write_DS1302(charger,0xa2);      //charger size
write_DS1302(WRITE_SECOND,sec); //second
write_DS1302(WRITE_MINUTE,min); //minute
write_DS1302(WRITE_HOUR,hour);   //hours
write_DS1302(WRITE_DAY,day);    //day
write_DS1302(WRITE_MONTH,month);  //month
write_DS1302(WRITE_YEARS,year);  //years

write_DS1302(WRITE_PROTECT,0x80);//open write baohu


}



	
