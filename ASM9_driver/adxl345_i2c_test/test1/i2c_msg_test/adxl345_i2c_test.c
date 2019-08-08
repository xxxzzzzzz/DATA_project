#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/jiffies.h>
#include <linux/i2c.h>
#include <linux/delay.h>

/////////////////////////////////////////////register///////////////////
#define DEVICE_ID   0X00
#define THRESH_TAP  0X1D
#define OFSX        0X1E 
#define OFSY        0X1F
#define OFSZ        0X20
#define DUR         0X21
#define Latent      0X22
#define Window      0X23
#define THRESH_ACK  0X24
#define THRESH_INACT 0X25
#define TIME_INACT  0X26
#define ACT_INACT_CTL 0X27
#define THRESH_FF   0X28
#define TIME_FF     0X29
#define TAP_AXES    0X2A
#define ACT_TAP_STATUS 0X2B
#define BW_RATE     0X2C
#define POWER_CTL   0X2D
#define INT_ENABLE  0X2E
#define INT_MAP     0X2F
#define INT_SOURCE  0X30
#define DATA_FORMAT 0X31
#define DATA_X0     0X32
#define DATA_X1     0X33
#define DATA_Y0     0X34
#define DATA_Y1     0X35
#define DATA_Z0     0X36
#define DATA_Z1     0X37
#define FIFO_CTL    0X38
#define FIFO_STATUS 0X39
#define ADXL_READ   0XA7
#define ADXL_WRITE  0xA6
/////////////////////////////////////////////register///////////////////


struct i2c_client *adxl345_client;


static struct i2c_board_info __initdata adxl345_infd[] = {
	{
		I2C_BOARD_INFO("adxl345_i2c", 0x53),
	},
};


static void i2c_write_addr_data(unsigned char reg_addr,unsigned char value)
{
	struct i2c_msg msg[1];
	unsigned char buffer[2];
	int ret = -1;
	
	buffer[0] = reg_addr;
	buffer[1] = value;

	msg[0].addr = adxl345_client->addr;//从设备器件地址，从clinet的addr获取
	msg[0].buf = buffer;//将寄存器地址和写入寄存器的值发出去
	msg[0].len = 2; //就是计算buffer长度，buffer就是寄存器地址+数据
	msg[0].flags = 0;//flags = 0表示向器件写数据

	ret = i2c_transfer(adxl345_client->adapter,msg,1);
	if(ret == 1)
		printk("write i2c_transfer success\n");
	else
		printk("write i2c_transfer failed\n");
}


static unsigned char i2c_read_addr_data(unsigned char reg_addr)
{
	struct i2c_msg msg[2]; 
	//为什么msg要2个元素，因为I2C读函数，要求先确定读器件哪个地址，这个确定地址就是写地址，然后把地址的数据读出来
	unsigned char register_addr = reg_addr;
	unsigned char data;
	int ret = -1;

	/*************write read addr************/
	msg[0].addr = adxl345_client->addr;//从设备器件地址，从clinet的addr获取
	msg[0].buf = &register_addr;//要读取哪个寄存器地址的数据
	msg[0].len = 1; //这里只写1个寄存器，所以长度为1
	msg[0].flags = 0;//flags = 0表示向器件写数据
	
	
	/**********read addr on data*******/
	msg[1].addr = adxl345_client->addr;//从设备器件地址，从clinet的addr获取
	msg[1].buf = &data;//读出来的数据放在data地址上
	msg[1].len = 1; //读1个寄存器的数据给data，所以长度为1
	msg[1].flags = I2C_M_RD;//I2C_M_RD表示这次transfer传输为读数据

	ret = i2c_transfer(adxl345_client->adapter,msg,2);
	if(ret == 2)
		printk("read i2c_transfer success\n");
	else
		printk("read i2c_transfer failed\n");

	return data;
}


static int adxl345_probe(struct i2c_client *client, const struct i2c_device_id *id)
{
	static unsigned char ID;
	static unsigned char original_buf[6];
	static short x,y,z;

	printk("adxl345_probe\n");
	
	adxl345_client = client;
		
	ID = i2c_read_addr_data(DEVICE_ID);
	printk("adxl345 ID = %x\n",ID);

	i2c_write_addr_data(DATA_FORMAT,0x2b);
	i2c_write_addr_data(BW_RATE,0x0e);
	i2c_write_addr_data(POWER_CTL,0x28);
	i2c_write_addr_data(INT_ENABLE,0x00);
	i2c_write_addr_data(OFSX,0x00);
	i2c_write_addr_data(OFSY,0x00);
	i2c_write_addr_data(OFSZ,0x00);

	while(1)
	{
		original_buf[0] = i2c_read_addr_data(0x32);
		original_buf[1] = i2c_read_addr_data(0x33);
		original_buf[2] = i2c_read_addr_data(0x34);
		original_buf[3] = i2c_read_addr_data(0x35);
		original_buf[4] = i2c_read_addr_data(0x36);
		original_buf[5] = i2c_read_addr_data(0x37);
		x=(short)(((unsigned int)original_buf[1]<<8)+original_buf[0]);
		y=(short)(((unsigned int)original_buf[3]<<8)+original_buf[2]);
		z=(short)(((unsigned int)original_buf[5]<<8)+original_buf[4]);
		printk("x = %d\r\n",x);
		printk("y = %d\r\n",y);
		printk("z = %d\r\n",z);
		mdelay(300);
	}
	return 0;	
}

static int __devexit adxl345_remove(struct i2c_client *client)
{
	printk("adxl345_remove\n");
	return 0;
}


static const struct i2c_device_id adxl345_id[] = {
	    { "adxl345_i2c", 0  },
	    {}
};


static struct i2c_driver adxl345_i2c_driver = {
	.driver = {
		.name = "xzz_i2c",
		.owner = THIS_MODULE,
	},
	.probe = adxl345_probe,
	.remove = __devexit_p(adxl345_remove),
	.id_table    = adxl345_id,
};


static int __init adxl345_i2c_init(void)
{
	int ret = -1;
	struct i2c_adapter *i2c_adap;

	ret = i2c_add_driver(&adxl345_i2c_driver);
	printk("i2c init ret = %d\n",ret);

	i2c_adap = i2c_get_adapter(1);
	adxl345_client = i2c_new_device(i2c_adap,adxl345_infd);

	return ret;
}

static void __exit adxl345_i2c_exit(void)
{
	i2c_unregister_device(adxl345_client);
	i2c_del_driver(&adxl345_i2c_driver);

}


module_init(adxl345_i2c_init);
module_exit(adxl345_i2c_exit);

MODULE_LICENSE("GPL");

