
#include <linux/device.h>

#include "iic_analog4.h"
#include <linux/gpio.h>
#include <linux/delay.h>
#include <linux/io.h>
#include <linux/of_gpio.h>
#define IMX_GPIO_NR(bank, nr)		(((bank) - 1) * 32 + (nr))
#define GPIO4_15 IMX_GPIO_NR(4, 15) 
#define GPIO4_14 IMX_GPIO_NR(4, 14) 

#define DOUT GPIO4_14
#define SCK  GPIO4_15 


unsigned long yuanshizhongliang4;
unsigned int ZLzhuanhuan_data4,fuweiz4; 



unsigned long Read_HX711_4(void){
	
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


//////////////////get_pressure4/////
unsigned long get_weight_4(void){
	

unsigned int count=0;
unsigned long sum=0;
unsigned long zhongliang;
for(count=0;count<10;count++){
	
zhongliang=Read_HX711_4();
yuanshizhongliang4=zhongliang;
zhongliang=(~zhongliang);
zhongliang=zhongliang>>8;
sum+=zhongliang;


}
zhongliang=sum/10;
return zhongliang;

}

