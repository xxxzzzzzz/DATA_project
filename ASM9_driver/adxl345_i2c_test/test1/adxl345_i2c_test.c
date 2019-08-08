#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/jiffies.h>
#include <linux/i2c.h>
#include <linux/mutex.h>

struct i2c_client *adxl345_client;

static int adxl345_probe(struct i2c_client *client, const struct i2c_device_id *id)
{
	printk("adxl345_probe\n");
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

