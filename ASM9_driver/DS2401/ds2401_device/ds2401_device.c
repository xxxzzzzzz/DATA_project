#include<linux/init.h>
#include<linux/module.h>
#include<linux/platform_device.h>

#define DEBUG


struct ds2401_GPIO_definit
{
	int ds2401_data_GPIOgroup; //pin in where group
	int ds2401_data_pin; //pin group to single pin
};

static struct ds2401_GPIO_definit ds2401_data = 
{
	.ds2401_data_GPIOgroup = 8,
	.ds2401_data_pin = 1,
};

void ds2401_release(struct device *dev)
{

}

struct platform_device ds2401_device = 
{
	.name = "ds2401_driver_device",
	.id = -1,
		.dev = {
			.platform_data = &ds2401_data,
			.release = ds2401_release,
		},
};

static int __init ds2401_init(void)
{
	platform_device_register(&ds2401_device);
#ifdef DEBUG
       printk("ds2401_device init..\n");	
#endif
	return 0;
}

static void __exit ds2401_exit(void)
{
	platform_device_unregister(&ds2401_device);
#ifdef DEBUG
       printk("ds2401_device init..\n");	
#endif

}

module_init(ds2401_init);
module_exit(ds2401_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("xzz");


