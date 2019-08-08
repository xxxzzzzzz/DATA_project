#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/jiffies.h>
#include <linux/i2c.h>
#include <linux/mutex.h>


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


static struct i2c_client *adxl345_client;

static int adxl345_probe(struct i2c_client *client, const struct i2c_device_id *id)
{
	unsigned int ret = -1; 
	static unsigned char ID;
	printk("adxl345_probe\n");

	adxl345_client = client;
	ret = i2c_check_functionality(adxl345_client->adapter,I2C_FUNC_SMBUS_BYTE_DATA);
	if(ret < 0)
		printk("I2C not supported SMBUS\n");
	else	
		printk("I2C supported SMBUS success\n");
	

	ID = i2c_smbus_read_byte_data(adxl345_client,DEVICE_ID);
	printk("adxl345 ID = %x\n",ID);
	return 0;	
}

static int __devexit adxl345_remove(struct i2c_client *client)
{
	printk("adxl345_remove\n");
	return 0;
}

static struct i2c_board_info __initdata adxl345_infd[] = {
	{
		I2C_BOARD_INFO("adxl345_i2c", 0x53),
	},
};


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
	i2c_put_adapter(i2c_adap);

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

