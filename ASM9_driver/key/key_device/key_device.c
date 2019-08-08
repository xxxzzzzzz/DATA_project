#include<linux/init.h>
#include<linux/module.h>
#include<linux/platform_device.h>

#define DEBUG


struct key_GPIO_definit
{
	int key_group;
	int key_gpio_pin;
};

static struct key_GPIO_definit key_data = 
{
	.key_group = 17,
	.key_gpio_pin = 6,
};

void key_release(struct device *dev)
{

}

struct platform_device key_device = 
{
	.name = "key_driver_device",
	.id = -1,
		.dev = {
			.platform_data = &key_data,
			.release = key_release,
		},
};

static int __init key_init(void)
{
	platform_device_register(&key_device);
#ifdef DEBUG
       printk("key_device init..\n");	
#endif
	return 0;
}

static void __exit key_exit(void)
{
	platform_device_unregister(&key_device);
#ifdef DEBUG
       printk("key_device init..\n");	
#endif

}

module_init(key_init);
module_exit(key_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("xzz");


