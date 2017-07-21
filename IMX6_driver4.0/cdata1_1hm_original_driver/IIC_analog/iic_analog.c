
#include <linux/device.h>

#include "iic_analog.h"
#include <linux/gpio.h>
#include <linux/delay.h>
#include <linux/io.h>
#include <linux/of_gpio.h>
#define IMX_GPIO_NR(bank, nr)		(((bank) - 1) * 32 + (nr))

#define GPIO5_14 IMX_GPIO_NR(5, 14)
#define GPIO5_15 IMX_GPIO_NR(5, 15)

#define DOUT GPIO5_14
#define SCK  GPIO5_15 

unsigned long yuanshizhongliang;
unsigned int ZLzhuanhuan_data,fuweiz;



unsigned long Read_HX711(void){
	
unsigned long val=0;
unsigned char i=0;
 gpio_direction_output(DOUT,1);
 gpio_direction_input(DOUT); 
 gpio_direction_output(SCK,0);
 while(gpio_get_value(DOUT)!=0);
 udelay(1);
 for(i=0;i<24;i++){
	
 gpio_set_value(SCK,1);//SCK=1
 val=val<<1;
 udelay(1);
 gpio_set_value(SCK,0);//SCK=0
 if(gpio_get_value(DOUT)!=0)
 val++;
 udelay(1);
         
   }

 gpio_set_value(SCK,1);//SCK=1  
 val=val^0x800000;
 udelay(1);
 gpio_set_value(SCK,0);//SCK=0 
 udelay(1);
return val;

}


//////////////////get_pressure/////
unsigned long get_weight(void){
	

unsigned int count=0;
unsigned long sum=0;
unsigned long zhongliang;
for(count=0;count<10;count++){
	
zhongliang=Read_HX711();
yuanshizhongliang=zhongliang;
zhongliang=(~zhongliang);
zhongliang=zhongliang>>8;
sum+=zhongliang;


}
zhongliang=sum/10;
return zhongliang;

}

