#include <linux/device.h>
#include <linux/gpio.h>
#include <linux/delay.h>
#include <linux/io.h>
#include<mach/pincontrol.h>
//#define DEBUG
void DHT11_Rst(int gpio_group,int gpio_pin)
{ 
    write_GPIO(gpio_group,gpio_pin,1); //dht11_data = 1 这里拉高避免应用层OPEN的时候出现错误
    mdelay(1);  
    write_GPIO(gpio_group,gpio_pin,0); //dht11_data = 0
    mdelay(25);    	//拉低至少18ms
    write_GPIO(gpio_group,gpio_pin,1); //dht11_data = 1
    udelay(30);     	//主机拉高20~40us
}

//等待DHT11的回应
//返回1:未检测到DHT11的存在
//返回0:存在
unsigned char  DHT11_Check(int gpio_group,int gpio_pin) 	   
{
      unsigned long retry=0;
      while(read_GPIO(gpio_group,gpio_pin)&&retry<100)//DHT11会拉低40~
      {
		retry++;
		udelay(1);
      } 
	      //printk("retry = %d\n",retry);
      if(retry>=100)
      {
	#ifdef DEBUG
	      printk("check 1\n");
	#endif
	      return 1;
      }
      else 
	      retry=0;

      while (!read_GPIO(gpio_group,gpio_pin)&&retry<100)//DHT11拉低后会再次拉高40~80us
      {
	       retry++;
	       udelay(1);
      }
      if(retry>=100)
      {
#ifdef DEBUG
	      printk("check 2\n"); 
#endif
	      return 1;
      }
      return 0;
}

//从DHT11读取一个位
////返回值：1/0
unsigned char DHT11_Read_Bit(int gpio_group,int gpio_pin){
      unsigned char retry=0;
      while(read_GPIO(gpio_group,gpio_pin)&&retry<100)//等待变为低电平
      {
	
	retry++;
	udelay(1);
      }	
        retry=0;
      while(!read_GPIO(gpio_group,gpio_pin)&&retry<100)//等待变高电平
      {
	
	retry++;
	udelay(1);
      }
       	udelay(40);//等待40us
     if(read_GPIO(gpio_group,gpio_pin))return 1;
     else return 0;
} 

//从DHT11读取一个字节
//返回值：读到的数据
unsigned char DHT11_Read_Byte(int gpio_group,int gpio_pin){
     unsigned char i,dat;
     dat=0;
     for(i=0;i<8;i++) 
     {
       dat<<=1; 
       dat|=DHT11_Read_Bit(gpio_group,gpio_pin);
					        
     }

    write_GPIO(gpio_group,gpio_pin,1); //dht11_data = 1 DHT11完成一次发送要拉高数据线
     return dat;
}


//从DHT11读取一次数据
//temp:温度值(范围:0~50°)
//humi:湿度值(范围:20%~90%)
//返回值：0,正常;1,读取失败
unsigned char  DHT11_Read_Data(unsigned char *temp,unsigned char *humi,int gpio_group,int gpio_pin){
	
   unsigned char buff[5];
   unsigned char i;
   DHT11_Rst(gpio_group,gpio_pin);
   if(DHT11_Check(gpio_group,gpio_pin)==0)
   {
	
	for(i=0;i<5;i++)//读取40位数据
	{
		buff[i]=DHT11_Read_Byte(gpio_group,gpio_pin);
	}
	if((buff[0]+buff[1]+buff[2]+buff[3])==buff[4]){
		
		*humi=buff[0];
		*temp=buff[2];
#ifdef DEBUG
		printk("kernel buff[0] = %d buff[2] = %d\n",buff[0],buff[2]);
#endif
	}
	else 
		return -1;
   }
	return 0;

}

//初始化DHT11的IO口 DQ 同时检测DHT11的存在
//返回1:不存在
//返回0:存在    	 
unsigned char DHT11_Init(int gpio_group,int gpio_pin){
DHT11_Rst(gpio_group,gpio_pin);  //复位DHT11
return DHT11_Check(gpio_group,gpio_pin);//等待DHT11的回应
}
