
#include <linux/device.h>

#include <linux/gpio.h>
#include <linux/delay.h>
#include <linux/io.h>
#include <linux/of_gpio.h>
#include "temphumi.h"

#define IMX_GPIO_NR(bank, nr)		(((bank) - 1) * 32 + (nr))
#define GPIO1_2 IMX_GPIO_NR(1, 2) 
#define DHT11_DQ GPIO1_2

void DHT11_Rst(void)
{   
    gpio_direction_output(DHT11_DQ,0); 	//SET OUTPUT
    gpio_set_value(DHT11_DQ,0); 	//拉低DQ
    mdelay(20);    	//拉低至少18ms
    gpio_set_value(DHT11_DQ,1); 	//DQ=1 
    udelay(30);     	//主机拉高20~40us
}

//等待DHT11的回应
////返回1:未检测到DHT11的存在
////返回0:存在
unsigned char  DHT11_Check(void) 	   
{
      unsigned char retry=0;
      gpio_direction_input(DHT11_DQ);
      while(gpio_get_value(DHT11_DQ)&&retry<100)//DHT11会拉低40~
      {
		retry++;
		udelay(1);
      } 
      if(retry>=100)return 1;
      else retry=0;
      while (!gpio_get_value(DHT11_DQ)&&retry<100)//DHT11拉低后会再次拉高40~80us
      {
	       retry++;
	       udelay(1);
      }
      if(retry>=100)return 1;
      return 0;
}

//从DHT11读取一个位
////返回值：1/0
unsigned char DHT11_Read_Bit(void){
      unsigned char retry=0;
      gpio_direction_input(DHT11_DQ);
      while(gpio_get_value(DHT11_DQ)&&retry<100)//等待变为低电平
      {
	
	retry++;
	udelay(1);
      }	
        retry=0;
      while(!gpio_get_value(DHT11_DQ)&&retry<100)//等待变高电平
      {
	
	retry++;
	udelay(1);
      }
       udelay(40);//等待40us
     if(gpio_get_value(DHT11_DQ))return 1;
     else return 0;
} 


//从DHT11读取一个字节
////返回值：读到的数据
unsigned char DHT11_Read_Byte(void){
     unsigned char i,dat;
     dat=0;
     for(i=0;i<8;i++) 
     {
       dat<<=1; 
       dat|=DHT11_Read_Bit();
					        
     }
     return dat;
}

//从DHT11读取一次数据
////temp:温度值(范围:0~50°)
////humi:湿度值(范围:20%~90%)
////返回值：0,正常;1,读取失败
unsigned char  DHT11_Read_Data(unsigned char *temp,unsigned char *humi){
	
    unsigned char buff[5];
    unsigned char i;
    DHT11_Rst();
   if(DHT11_Check()==0)
   {
	
	for(i=0;i<5;i++)//读取40位数据
	{
		buff[i]=DHT11_Read_Byte();
	}
	if((buff[0]+buff[1]+buff[2]+buff[3])==buff[4]){
		
		*humi=buff[0];
		*temp=buff[2];
	}else return 1;
	return 0;
   }



}

//初始化DHT11的IO口 DQ 同时检测DHT11的存在
//返回1:不存在
////返回0:存在    	 
unsigned char DHT11_Init(void){
DHT11_Rst();  //复位DHT11
return DHT11_Check();//等待DHT11的回应

}
