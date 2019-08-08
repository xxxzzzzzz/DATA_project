#include<linux/init.h>
#include<linux/module.h>
#include<linux/platform_device.h>

#define DEBUG


static struct ds1302_GPIO_definit
{
	int ds1302_IO_GPIOgroup; //DS1302 data
	int ds1302_IO_pin; //DS1302 data
	int ds1302_SCLK_GPIOgroup; //DS1302 data sclk
	int ds1302_SCLK_pin; //DS1302 data sclk
	int ds1302_CE_GPIOgroup; //DS1302 data cs
	int ds1302_CE_pin; //DS1302 data cs
};

static struct ds1302_GPIO_definit ds1302_data = 
{
	.ds1302_IO_GPIOgroup = 4,
	.ds1302_IO_pin = 6,
	.ds1302_SCLK_GPIOgroup = 4,
	.ds1302_SCLK_pin = 5,
	.ds1302_CE_GPIOgroup = 4,
	.ds1302_CE_pin = 7,
};

void ds1302_release(struct device *dev)
{

}

struct platform_device ds1302_device = 
{
	.name = "ds1302_driver_device",
	.id = -1,
		.dev = {
			.platform_data = &ds1302_data,
			.release = ds1302_release,
		},
};

static int __init ds1302_init(void)
{
	platform_device_register(&ds1302_device);
#ifdef DEBUG
       printk("ds1302_device init..\n");	
#endif
	return 0;
}

static void __exit ds1302_exit(void)
{
	platform_device_unregister(&ds1302_device);
#ifdef DEBUG
       printk("ds1302_device exit..\n");	
#endif

}

module_init(ds1302_init);
module_exit(ds1302_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("xzz");


