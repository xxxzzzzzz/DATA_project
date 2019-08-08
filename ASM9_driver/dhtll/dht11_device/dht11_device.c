#include<linux/init.h>
#include<linux/module.h>
#include<linux/platform_device.h>

#define DEBUG


struct dht11_GPIO_definit
{
	int dht11_data_GPIOgroup; //pin in where group
	int dht11_data_pin; //pin group to single pin
};

static struct dht11_GPIO_definit dht11_data = 
{
	.dht11_data_GPIOgroup = 0,
	.dht11_data_pin = 3,
};

void dht11_release(struct device *dev)
{

}

struct platform_device dht11_device = 
{
	.name = "dht11_driver_device",
	.id = -1,
		.dev = {
			.platform_data = &dht11_data,
			.release = dht11_release,
		},
};

static int __init dht11_init(void)
{
	platform_device_register(&dht11_device);
#ifdef DEBUG
       printk("dht11_device init..\n");	
#endif
	return 0;
}

static void __exit dht11_exit(void)
{
	platform_device_unregister(&dht11_device);
#ifdef DEBUG
       printk("dht11_device init..\n");	
#endif

}

module_init(dht11_init);
module_exit(dht11_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("xzz");


