#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/jiffies.h>
#include <linux/i2c.h>
#include <linux/mutex.h>
#include <linux/bcd.h>
#include <linux/delay.h>

#define CONTROL1_addr 0x0e
#define CONTROL2_addr 0x0f


static struct i2c_client *RX8025_client;

struct RX8025_Time
{
	unsigned char sec;
	unsigned char min;
	unsigned char hours;
	unsigned char day;
	unsigned char week;
	unsigned char month;
	unsigned char year;	

};

static void i2c_write_addr_data(unsigned char reg_addr,unsigned char value)
{
	struct i2c_msg msg[1];
	unsigned char buffer[2];
	int ret = -1;
	
	buffer[0] = reg_addr;
	buffer[1] = value;

	msg[0].addr = RX8025_client->addr;//从设备器件地址，从clinet的addr获取
	msg[0].buf = buffer;//将寄存器地址和写入寄存器的值发出去
	msg[0].len = 2; //就是计算buffer长度，buffer就是寄存器地址+数据
	msg[0].flags = 0;//flags = 0表示向器件写数据

	ret = i2c_transfer(RX8025_client->adapter,msg,1);
/*	if(ret == 1)
	{

		printk("write i2c_transfer success\n");	
	}
	else
	{
		printk("write i2c_transfer failed\n");	
	}*/	
}


static unsigned char i2c_read_addr_data(unsigned char reg_addr)
{
	struct i2c_msg msg[2]; 
	//为什么msg要2个元素，因为I2C读函数，要求先确定读器件哪个地址，这个确定地址就是写地址，然后把地址的数据读出来
	unsigned char register_addr = reg_addr;
	unsigned char data;
	int ret = -1;

	/*************write read addr************/
	msg[0].addr = RX8025_client->addr;//从设备器件地址，从clinet的addr获取
	msg[0].buf = &register_addr;//要读取哪个寄存器地址的数据
	msg[0].len = 1; //这里只写1个寄存器，所以长度为1
	msg[0].flags = 0;//flags = 0表示向器件写数据
	
	
	/**********read addr on data*******/
	msg[1].addr = RX8025_client->addr;//从设备器件地址，从clinet的addr获取
	msg[1].buf = &data;//读出来的数据放在data地址上
	msg[1].len = 1; //读1个寄存器的数据给data，所以长度为1
	msg[1].flags = I2C_M_RD;//I2C_M_RD表示这次transfer传输为读数据

	ret = i2c_transfer(RX8025_client->adapter,msg,2);
/*	if(ret == 2)
	{

		printk("read i2c_transfer success\n");
	}
	else
	{
		printk("read i2c_transfer failed\n");
	}*/
	return data;
}
static unsigned char RX8025_Read_reg(unsigned char reg_addr)
{
	return i2c_smbus_read_byte_data(RX8025_client,reg_addr);	
	//return i2c_read_addr_data(reg_addr);

}

static void RX8025_Write_reg(unsigned char reg_addr,unsigned char val)
{

	i2c_smbus_write_byte_data(RX8025_client, reg_addr,val);	
	//i2c_write_addr_data(reg_addr,val);
}


static void RX8025_Time_set(struct RX8025_Time *tm)
{
	unsigned char temp = 0,i = 0;
	unsigned char rtc_str[7]={0};

	if((RX8025_Read_reg(CONTROL1_addr)&0x20)!=0X20)
	{
		temp = RX8025_Read_reg(CONTROL1_addr);
		temp |= 1<<5;
		RX8025_Write_reg(CONTROL1_addr,temp);
		printk("write CONTROL1 = %0x\n",temp);
	}

	temp = RX8025_Read_reg(CONTROL1_addr);
	printk("set CONTROL1 = %0x\n",temp);

	rtc_str[0] = bin2bcd(tm->sec);
	rtc_str[1] = bin2bcd(tm->min);
	rtc_str[2] = bin2bcd(tm->hours);
	rtc_str[3] = bin2bcd(tm->week);
	rtc_str[4] = bin2bcd(tm->day);
	rtc_str[5] = bin2bcd(tm->month);
	rtc_str[6] = bin2bcd(tm->year);

	for(i = 0;i < 7;i++)
	{
		RX8025_Write_reg(i,rtc_str[i]);
	}

}


static void RX8025_get_Time(struct RX8025_Time *tm)
{

	unsigned char temp = 0,i = 0;
	unsigned char rtc_str[7]={0};

	if((RX8025_Read_reg(CONTROL1_addr)&0x20)!=0x20)
	{
		temp = RX8025_Read_reg(CONTROL1_addr);
		temp |= 1<<5;
		RX8025_Write_reg(CONTROL1_addr,temp);	
	}
	
	temp = RX8025_Read_reg(CONTROL1_addr);
	printk("get time CONTROL1 = %x\n",temp);

	for(i = 0;i < 7;i++)
	{
		rtc_str[i] = RX8025_Read_reg(i);
	}

	tm->sec = bcd2bin(rtc_str[0]);
	tm->min = bcd2bin(rtc_str[1]);
	tm->hours = bcd2bin(rtc_str[2]);
	tm->week = bcd2bin(rtc_str[3]);
	tm->day = bcd2bin(rtc_str[4]);
	tm->month = bcd2bin(rtc_str[5]);
	tm->year = bcd2bin(rtc_str[6]);
}


static __devinit int RX8025_probe(struct i2c_client *i2c,const struct i2c_device_id *id)
{
	struct RX8025_Time Settm;
	struct RX8025_Time Gettm;
	unsigned int ret = -1;
	unsigned char secc = 0,temp = 0;

	RX8025_client = i2c;

	printk("RX8025_probe\n");

	Settm.sec = 50;
	Settm.min = 59;
	Settm.hours = 10;
	Settm.day = 10;
	Settm.week = 1;
	Settm.month = 10;
	Settm.year = 10;

	RX8025_Time_set(&Settm);

	while(1)
	{
		RX8025_get_Time(&Gettm);
		printk("sec = %d min = %d hours = %d day = %d week = %d month = %d year = %d\n",Gettm.sec,Gettm.min,Gettm.hours,Gettm.day,Gettm.week,Gettm.month,Gettm.year);
		mdelay(100);
	}
	return 0;
}

static __devexit int RX8025_remove(struct i2c_client *client)
{
	printk("RX8025_remove\n");
	return 0;
}


static const struct i2c_device_id RX8025_i2c_id[] = {
	{ "RX8025_i2c", 0  },
	{ }
};



static struct i2c_driver RX8025_i2c_driver = {
	.driver = {
		.name = "RX8025_i2c",
		.owner = THIS_MODULE,
	},
	.probe = RX8025_probe,
	.remove = __devexit_p(RX8025_remove),
	.id_table    = RX8025_i2c_id,
};


static struct i2c_board_info __initdata RX8025_infd[] = {                                                                                             
	{
		I2C_BOARD_INFO("RX8025_i2c", 0x32),       
	},
};


static int __init RX8025_i2c_init(void)
{
	int ret = -1;
	struct i2c_adapter *i2c_adap;

	ret = i2c_add_driver(&RX8025_i2c_driver);
	if(ret < 0)
		printk("RX8025 init module failed\n");
	else
		printk("RX8025 init module success");


	i2c_adap = i2c_get_adapter(1);
	RX8025_client = i2c_new_device(i2c_adap,RX8025_infd);
	if(RX8025_client)
		printk("RX8025 device exist\n");
	else
		printk("RX8025 not exist\n");


	
	return ret;
}

static void __exit RX8025_i2c_exit(void)
{
	i2c_unregister_device(RX8025_client);
	i2c_del_driver(&RX8025_i2c_driver);
	printk("RX8025 exit module\n");
}


module_init(RX8025_i2c_init);
module_exit(RX8025_i2c_exit);

MODULE_LICENSE("GPL");

