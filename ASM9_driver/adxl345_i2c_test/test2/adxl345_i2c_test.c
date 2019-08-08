#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/jiffies.h>
#include <linux/i2c.h>
#include <linux/mutex.h>

#define DEVICE_ID 0x00

static struct i2c_client *adxl345_client;

static int adxl345_probe(struct i2c_client *client, const struct i2c_device_id *id)
{
	unsigned int ret = -1; 
	static unsigned char ID;
	printk("adxl345_probe\n");

	ret = i2c_check_functionality(client->adapter,I2C_FUNC_SMBUS_BYTE_DATA);
	if(ret < 0)
		printk("I2C not supported SMBUS\n");
	else	
		printk("I2C supported SMBUS success\n");
	
	ID = i2c_smbus_read_byte_data(client,DEVICE_ID);
	printk("adxl345 ID = %x\n",ID);
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
		.name = "adxl345_i2c",
		.owner = THIS_MODULE,
	},
	.probe = adxl345_probe,
	.remove = __devexit_p(adxl345_remove),
	.id_table    = adxl345_id,
};

static const unsigned short device_addr[] = {0x29,0x51,0x53,I2C_CLIENT_END};

static int __init adxl345_i2c_init(void)
{
	int ret = -1;
	struct i2c_adapter *i2c_adap;
	struct i2c_board_info adxl345_info;


	memset(&adxl345_info,0,sizeof(struct i2c_board_info));
	strlcpy(adxl345_info.type,"adxl345_i2c",I2C_NAME_SIZE);

	i2c_adap = i2c_get_adapter(1);
	adxl345_client = i2c_new_probed_device(i2c_adap,&adxl345_info,device_addr);
	i2c_put_adapter(i2c_adap);
	
	if(adxl345_client)
		printk("device exist\n");
	else
		printk("device not exist\n");
	ret = i2c_add_driver(&adxl345_i2c_driver);
	printk("i2c init ret = %d\n",ret);
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

