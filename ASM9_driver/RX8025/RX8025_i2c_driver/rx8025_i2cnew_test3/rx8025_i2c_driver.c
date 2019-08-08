#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/jiffies.h>
#include <linux/i2c.h>
#include <linux/delay.h>
#include <linux/bcd.h>

#define RX8025_REG_CTRL1     ( 0x0e  )
#define RX8025_REG_CTRL2     ( 0x0f  )

static struct i2c_client *rx8025_client;


static struct i2c_board_info __initdata rx8025_infd[] = {
	{
		I2C_BOARD_INFO("rx8025_i2c", 0x32),
	},
};




static int rx8025_probe(struct i2c_client *client, const struct i2c_device_id *id)
{
	unsigned int ret = -1,i = 0; 
	unsigned int data = 0;


	printk("rx8025_probe\n");

	ret = i2c_check_functionality(client->adapter,I2C_FUNC_SMBUS_BYTE_DATA);
	if(ret < 0)
		printk("I2C not supported SMBUS\n");
	else	
		printk("I2C supported SMBUS success\n");

	udelay(60);
	data = i2c_smbus_read_byte_data(client,RX8025_REG_CTRL1);
	udelay(60);
	printk("control 1  = %x\n",data);
	data |= 1<<5;
	printk("contro1 write data = %x\n",data);
	ret = i2c_smbus_write_byte_data(client, RX8025_REG_CTRL1,data);
	udelay(60);
	data = i2c_smbus_read_byte_data(client,RX8025_REG_CTRL1);
	udelay(60);
	mdelay(1);
	printk("read control 1 = %x\n",data);

	data = i2c_smbus_read_byte_data(client,RX8025_REG_CTRL2);
	udelay(60);
	printk("control 2  = %x\n",data);

	ret = i2c_smbus_write_byte_data(client, RX8025_REG_CTRL1,0x20);
	udelay(60);
	ret = i2c_smbus_write_byte_data(client, RX8025_REG_CTRL2,0x20);
	udelay(60);
	ret = i2c_smbus_write_byte_data(client, 0x00,bin2bcd(0));
	udelay(60);
	ret = i2c_smbus_write_byte_data(client, 0x01,bin2bcd(2));
	udelay(60);
	ret = i2c_smbus_write_byte_data(client, 0x02,bin2bcd(3));
	udelay(60);
	ret = i2c_smbus_write_byte_data(client, 0x03,bin2bcd(4));
	udelay(60);
	ret =	i2c_smbus_write_byte_data(client, 0x04,bin2bcd(5));
	udelay(60);
	ret =	i2c_smbus_write_byte_data(client, 0x05,bin2bcd(6));
	udelay(60);
	ret =	i2c_smbus_write_byte_data(client, 0x06,bin2bcd(7));
	udelay(60);



	for(i = 0;i <7;i++)
	{
		
		data = i2c_smbus_read_byte_data(client,i);
		data = bcd2bin(data);
		printk("data = %x\n",data);
		udelay(60);
	}
	return 0;	
}

static int __devexit rx8025_remove(struct i2c_client *client)
{
	printk("rx8025_remove\n");
	return 0;
}


static const struct i2c_device_id rx8025_id[] = {
	    { "rx8025_i2c", 0  },
	    {}
};


static struct i2c_driver rx8025_i2c_driver = {
	.driver = {
		.name = "xzz_i2c",
		.owner = THIS_MODULE,
	},
	.probe = rx8025_probe,
	.remove = __devexit_p(rx8025_remove),
	.id_table    = rx8025_id,
};


static int __init rx8025_i2c_init(void)
{
	int ret = -1;
	struct i2c_adapter *i2c_adap;

	ret = i2c_add_driver(&rx8025_i2c_driver);
	printk("i2c init ret = %d\n",ret);

	i2c_adap = i2c_get_adapter(1);
	rx8025_client = i2c_new_device(i2c_adap,rx8025_infd);

	return ret;
}

static void __exit rx8025_i2c_exit(void)
{
	i2c_unregister_device(rx8025_client);
	i2c_del_driver(&rx8025_i2c_driver);

}


module_init(rx8025_i2c_init);
module_exit(rx8025_i2c_exit);

MODULE_LICENSE("GPL");

