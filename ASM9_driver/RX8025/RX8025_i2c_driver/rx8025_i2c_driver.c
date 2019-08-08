#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/jiffies.h>
#include <linux/i2c.h>
#include <linux/mutex.h>
#include <linux/bcd.h>
#include <linux/delay.h>

#define RX8025_REG_SC        ( 0x00  ) /* Second in BCD */
#define RX8025_REG_MN        ( 0x01  ) /* Minute in BCD */
#define RX8025_REG_HR        ( 0x02  ) /* Hour in BCD */
#define RX8025_REG_DW        ( 0x03  ) /* Day of Week */
#define RX8025_REG_DM        ( 0x04  ) /* Day of Month in BCD */
#define RX8025_REG_MO        ( 0x05  ) /* Month in BCD */
#define RX8025_REG_YR        ( 0x06  ) /* Year in BCD */
#define RX8025_REG_RAM       ( 0x07  ) /* Dlgital Offset */
#define RX8025_REG_AMN       ( 0x08  ) /* Alarm Min in BCD*/
#define RX8025_REG_CTRL1     ( 0x0e  )
#define RX8025_REG_CTRL2     ( 0x0f  )


#define CONTROL1_addr 0x0e


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

static unsigned char RX8025_Read_reg(struct i2c_client *client,unsigned char reg_addr)
{
	return i2c_smbus_read_byte_data(client,reg_addr);	
}

static void RX8025_Write_reg(struct i2c_client *client,unsigned char reg_addr,unsigned char val)
{

	i2c_smbus_write_byte_data(client, reg_addr,val);	
}


static void RX8025_single_reg(struct i2c_client *client)
{
	unsigned char data = 0;
	unsigned char temp = 0;

/*	if(RX8025_Read_reg(client,CONTROL1_addr)!=0X20)
	{
		temp = RX8025_Read_reg(client,CONTROL1_addr);
		temp |= 1<<5;
		i2c_smbus_write_byte_data(client,CONTROL1_addr,temp);
	}*/
	
	i2c_smbus_write_byte_data(RX8025_client, RX8025_REG_CTRL1,0x20);
	i2c_smbus_write_byte_data(RX8025_client, RX8025_REG_CTRL2,0x20);

	RX8025_Write_reg(RX8025_client,0x01,0x04);
	mdelay(100);
	data = RX8025_Read_reg(RX8025_client,0x01);
	//data = bcd2bin(data);
	printk("data = %x\n",data);
}

static void RX8025_Time_set(struct i2c_client *client,struct RX8025_Time *tm)
{
	unsigned char temp = 0,i = 0;
	unsigned char rtc_str[7]={0};
	int err;

	if(RX8025_Read_reg(client,CONTROL1_addr)!=0X20)
	{
		temp = RX8025_Read_reg(client,CONTROL1_addr);
		temp |= 1<<5;
		i2c_smbus_write_byte_data(client,CONTROL1_addr,temp);
	}

/*	rtc_str[0] = bin2bcd(tm->sec);
	rtc_str[1] = bin2bcd(tm->min);
	rtc_str[2] = bin2bcd(tm->hours);
	rtc_str[3] = bin2bcd(tm->week);
	rtc_str[4] = bin2bcd(tm->day);
	rtc_str[5] = bin2bcd(tm->month);
	rtc_str[6] = bin2bcd(tm->year);

	for(i = 0;i < 7;i++)
	{
		RX8025_Write_reg(client,i,rtc_str[i]);
	}*/

	err = i2c_smbus_write_byte_data(client, RX8025_REG_SC,bin2bcd(tm->sec));
	if (err < 0) {
		printk("write failed\n");	    
	}

	err = i2c_smbus_write_byte_data(client, RX8025_REG_MN,bin2bcd(tm->min));
	if (err < 0) {
		printk("write failed\n");	    
	}
	err = i2c_smbus_write_byte_data(client, RX8025_REG_HR,bin2bcd(tm->hours));
	if (err < 0) {
		printk("write failed\n");		    
	}
	err = i2c_smbus_write_byte_data(client, RX8025_REG_DW,bin2bcd(tm->week));
	if (err < 0) {
		printk("write failed\n");       
	}
	err = i2c_smbus_write_byte_data(client, RX8025_REG_DM,bin2bcd(tm->day));
	if (err < 0) {
		printk("write failed\n");    
	}
	err = i2c_smbus_write_byte_data(client, RX8025_REG_MO,bin2bcd(tm->month+1));
	if (err < 0) {
		printk("write failed\n");
	}
	err = i2c_smbus_write_byte_data(client, RX8025_REG_YR,bin2bcd(tm->year%100));
	if (err < 0) {
		printk("write failed\n");
	}

}


static void RX8025_get_Time(struct i2c_client *client,struct RX8025_Time *tm)
{

	unsigned char temp = 0,i = 0;
	unsigned char rtc_str[7]={0};
	int data;

	i2c_smbus_write_byte_data(client, RX8025_REG_CTRL1,0x20);
	i2c_smbus_write_byte_data(client, RX8025_REG_CTRL2,0x20);

	if((RX8025_Read_reg(client,CONTROL1_addr)&0x20)!=0x20)
	{
		temp = RX8025_Read_reg(client,CONTROL1_addr);
		temp |= 1<<5;
		RX8025_Write_reg(client,CONTROL1_addr,temp);	
	}

/*	for(i = 0;i < 7;i++)
	{
		rtc_str[i] = RX8025_Read_reg(client,i);
	}*/

	data = i2c_smbus_read_byte_data(client, RX8025_REG_SC);
	tm->sec = bcd2bin(data);

	data = i2c_smbus_read_byte_data(client, RX8025_REG_MN);
	tm->min = bcd2bin(data);

	data = i2c_smbus_read_byte_data(client, RX8025_REG_HR);
	tm->hours = bcd2bin(data & 0x3F);

	data = i2c_smbus_read_byte_data(client, RX8025_REG_DW);
	tm->week = bcd2bin(data & 0x7);

	data = i2c_smbus_read_byte_data(client, RX8025_REG_DM);
	tm->day = bcd2bin(data & 0x3F);

	data = i2c_smbus_read_byte_data(client, RX8025_REG_MO);
	tm->month = bcd2bin(data & 0x1F) - 1;

	data = i2c_smbus_read_byte_data(client, RX8025_REG_YR);
	tm->year = bcd2bin(data);

/*	tm->sec = bcd2bin(rtc_str[0]);
	tm->min = bcd2bin(rtc_str[1]);
	tm->hours = bcd2bin(rtc_str[2]);
	tm->week = bcd2bin(rtc_str[3]);
	tm->day = bcd2bin(rtc_str[4]);
	tm->month = bcd2bin(rtc_str[5]);
	tm->year = bcd2bin(rtc_str[6]);*/
}


static __devinit int RX8025_probe(struct i2c_client *client,const struct i2c_device_id *id)
{
	struct RX8025_Time Settm;
	struct RX8025_Time Gettm;
	unsigned int ret = -1;

	//ret = i2c_check_functionality(i2c->adapter,I2C_FUNC_SMBUS_BYTE_DATA); 	
	ret = i2c_check_functionality(client->adapter,I2C_FUNC_I2C); 	
	if(ret < 0 )
		printk("I2C not supported SMBUS\n");
	else
		printk("I2C supported SMBUS success\n");

	Settm.sec = 50;
	Settm.min = 59;
	Settm.hours = 10;
	Settm.day = 10;
	Settm.week = 1;
	Settm.month = 10;
	Settm.year = 10;

//	RX8025_Time_set(client,&Settm);
	printk("RX8025_probe\n");
	mdelay(500);	
	RX8025_single_reg(client);
/*	while(1)
	{
		RX8025_get_Time(client,&Gettm);
		printk("sec = %d min = %d hours = %d day = %d week = %d month = %d year = %d\n",Gettm.sec,Gettm.min,Gettm.hours,Gettm.day,Gettm.week,Gettm.month,Gettm.year);
		mdelay(100);
	}*/
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

static const unsigned short RX8025_addr[] = {0x32,I2C_CLIENT_END}; //RX8025 chip addr = 0x32

static int __init RX8025_i2c_init(void)
{
	int ret = -1;
	struct i2c_adapter *i2c_adap;
	struct i2c_board_info RX8025_info;

	memset(&RX8025_info,0,sizeof(struct i2c_board_info));
	strlcpy(RX8025_info.type,"RX8025_i2c",I2C_NAME_SIZE);
	i2c_adap = i2c_get_adapter(1);
	RX8025_client = i2c_new_probed_device(i2c_adap,&RX8025_info,RX8025_addr);
	i2c_put_adapter(i2c_adap);
	
	if(RX8025_client)
		printk("RX8025 device exist\n");
	else
		printk("RX8025 not exist\n");


	ret = i2c_add_driver(&RX8025_i2c_driver);
	if(ret < 0)
		printk("RX8025 init module failed\n");
	else
		printk("RX8025 init module success");

	
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

