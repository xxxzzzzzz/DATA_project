#include<linux/init.h>
#include<linux/module.h>
#include<linux/platform_device.h>

#define DEBUG


struct usba_POWER_GPIO_definit
{
	int usba_POWER_group;
	int usba_POWER_gpio_pin;
};

static struct usba_POWER_GPIO_definit usba_POWER_data = 
{
	.usba_POWER_group = 2,
	.usba_POWER_gpio_pin = 2,
};

void usba_POWER_release(struct device *dev)
{

}

struct platform_device usba_POWER_device = 
{
	.name = "usba_POWER_driver_device",
	.id = -1,
		.dev = {
			.platform_data = &usba_POWER_data,
			.release = usba_POWER_release,
		},
};

static int __init usba_POWERdev_init(void)
{
	platform_device_register(&usba_POWER_device);
#ifdef DEBUG
       printk("usba_power_device init..\n");	
#endif
	return 0;
}

static void __exit usba_POWERdev_exit(void)
{
	platform_device_unregister(&usba_POWER_device);
#ifdef DEBUG
       printk("usba_power_device init..\n");	
#endif

}

module_init(usba_POWERdev_init);
module_exit(usba_POWERdev_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("xzz");


