#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/jiffies.h>
#include <linux/i2c.h>



static __devinit int adxl345_i2c_probe(struct i2c_client *i2c,const struct i2c_device_id *id)
{
	printk("adxl345_probe\n");
	return 0;
}

static __devexit int adxl345_i2c_remove(struct i2c_client *client)
{
	printk("adxl345_remove\n");
	return 0;
}


static const struct i2c_device_id adxl345_i2c_id[] = {
	{ "adxl345", 0  },
	{ }

};


static struct i2c_driver adxl345_i2c_driver = {
	.driver = {
			.name = "adxl345_i2c_driver",
			.owner = THIS_MODULE,
		  },
	.probe = adxl345_i2c_probe,
	.remove = __devexit_p(adxl345_i2c_remove),
	.id_table = adxl345_i2c_id,

};


static struct i2c_board_info __initdata adxl345_devices[] = {
	{
		I2C_BOARD_INFO("adxl345", 0x53)
	},
};


static int __init adxl345_i2c_init(void)
{
	i2c_register_board_info(1,adxl345_devices, ARRAY_SIZE(adxl345_devices));
	i2c_add_driver(&adxl345_i2c_driver);
	return 0;
}

static void __exit adxl345_i2c_exit(void)
{
	i2c_del_driver(&adxl345_i2c_driver);
}


module_init(adxl345_i2c_init);
module_exit(adxl345_i2c_exit);

MODULE_LICENSE("GPL");
