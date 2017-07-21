

#include <linux/device.h>

#include "bh1750_rw.h"
#include <linux/gpio.h>
#include <linux/delay.h>
#include <linux/io.h>
#include <linux/of_gpio.h>
#define IMX_GPIO_NR(bank, nr)		(((bank) - 1) * 32 + (nr))
#define GPIO1_3 IMX_GPIO_NR(1, 3) 
#define GPIO1_6 IMX_GPIO_NR(1, 6) 

#define clk GPIO1_3
#define sda GPIO1_6

static unsigned char ack,f0;
/* 起动总线函数              
   函数原型: void  Start_I2c();  
   功能:     启动I2C总线,即发送I2C起始条件. */
void Start_I2c(void){
gpio_direction_output(sda,1);
gpio_direction_output(clk,1);
gpio_set_value(sda,1);
gpio_set_value(clk,1);
udelay(4); 
gpio_set_value(sda,0);
udelay(4);
gpio_set_value(clk,0);
}

/*
    结束总线函数               
    函数原型: void  Stop_I2c();  
    功能:     结束I2C总线,即发送I2C结束条件.  
    */
void Stop_I2c(void){
gpio_direction_output(sda,1);
gpio_set_value(clk,0);
gpio_set_value(sda,0);
udelay(4);
gpio_set_value(clk,1);
gpio_set_value(sda,1);
udelay(4); 
}

//等待应答信号到来
////返回值：1，接收应答失败
////        0，接收应答成功
unsigned char IIC_Wait_Ack(void){
	
 unsigned char ucErrTime=0;
 gpio_direction_input(sda);  //SDA设置为输入
 gpio_set_value(sda,1);udelay(1);
 gpio_set_value(clk,1);udelay(1);
 while(gpio_get_value(sda)){

	 ucErrTime++;
	 if(ucErrTime>250){
           Stop_I2c();
	   return 1;

	 }
 }
gpio_set_value(clk,0);
return 0;
}



//产生ACK应答

void IIC_Ack(void){

   gpio_set_value(clk,0);
   
   gpio_direction_output(sda,0);
   gpio_set_value(sda,0);
   udelay(2);
   gpio_set_value(clk,1);
   udelay(2);
   gpio_set_value(clk,0);

}



//不产生ACK应答
void IIC_NAck(void){
   gpio_set_value(clk,0);
   gpio_direction_output(sda,1);
   gpio_set_value(sda,1);
   udelay(2);
   gpio_set_value(clk,1);
   udelay(2);
   gpio_set_value(clk,0);

}

//IIC发送一个字节
////返回从机有无应答
////1，有应答
////0，无应答
void IIC_Send_Byte(unsigned char txd){
	unsigned char t;
        gpio_direction_output(sda,1);
	gpio_set_value(clk,0); //拉低时钟开始数据传输
	for(t=0;t<8;t++){
	gpio_direction_output(sda,((txd&0x80)>>7));
	txd<<=1;
	udelay(2);
	 gpio_set_value(clk,1);
	 udelay(2);
	 gpio_set_value(clk,0);
	 udelay(2);

	}



}


//读1个字节，ack=1时，发送ACK，ack=0，发送nACK
unsigned char IIC_Read_Byte(unsigned char ack){
	unsigned char i,receive=0;
        gpio_direction_input(sda);  //SDA设置为输入
	for(i=0;i<8;i++){

	gpio_set_value(clk,0);
	udelay(2);
	gpio_set_value(clk,1);
	receive<<=1;
	if(gpio_get_value(sda))receive++;
	udelay(1);

	}
	if(!ack)
		IIC_NAck();//发送nACK
	else
		IIC_Ack(); //发送ACK
	return receive;



}

unsigned int bh1750(void){

unsigned int temp,i,buf[3];
unsigned char rxxd;
Start_I2c();

IIC_Send_Byte(0x46);
rxxd=IIC_Wait_Ack();
if(rxxd==1){printk("recive ack failed\n");}

IIC_Send_Byte(0x01);  //register addr
rxxd=IIC_Wait_Ack();
if(rxxd==1){printk("recive ack failed\n");}
Stop_I2c();

Start_I2c();
IIC_Send_Byte(0x46); 
rxxd=IIC_Wait_Ack();
if(rxxd==1){printk("recive ack failed\n");}

IIC_Send_Byte(0x10);
rxxd=IIC_Wait_Ack();
if(rxxd==1){printk("recive ack failed\n");}
Stop_I2c();

mdelay(180);

Start_I2c(); 
IIC_Send_Byte(0x47); 
rxxd=IIC_Wait_Ack();
if(rxxd==1){printk("recive ack failed\n");}

for(i=0;i<2;i++){
buf[i]=IIC_Read_Byte(1);
}

buf[2]=IIC_Read_Byte(0);
Stop_I2c();
mdelay(5);
temp=buf[0];
temp=(temp<<8)+buf[1];
return temp;
}


unsigned char PCF8591_WR_Reg2(void){
unsigned char rxxd,temp;
Start_I2c();

IIC_Send_Byte(0x90);
rxxd=IIC_Wait_Ack();
if(rxxd==1){printk("recive ack failed\n");}

IIC_Send_Byte(0x41);  //AIN1
rxxd=IIC_Wait_Ack();
if(rxxd==1){printk("recive ack failed\n");}

Start_I2c();
IIC_Send_Byte(0x91);
rxxd=IIC_Wait_Ack();
if(rxxd==1){printk("recive ack failed\n");}

temp=IIC_Read_Byte(0);
Stop_I2c();
return temp;


}
