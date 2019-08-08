#include<linux/init.h>
#include<linux/module.h>
#include<linux/platform_device.h>

#define DEBUG


struct hw_lowPowerCPUIO_GPIO_definit
{
	int hw_lowPowerCPUIO1_group;
	int hw_lowPowerCPUIO1_gpio_pin;
};

static struct hw_lowPowerCPUIO_GPIO_definit hw_lowPowerCPUIO_data = 
{
	.hw_lowPowerCPUIO1_group = 4,
	.hw_lowPowerCPUIO1_gpio_pin = 0,
};

void hw_lowPowerCPUIO_release(struct device *dev)
{

}

struct platform_device hw_lowPowerCPUIO_device = 
{
	.name = "hw_lowPowerCPUIO_driver_device",
	.id = -1,
		.dev = {
			.platform_data = &hw_lowPowerCPUIO_data,
			.release = hw_lowPowerCPUIO_release,
		},
};

static int __init hw_lowPowerCPUIO_init(void)
{
	platform_device_register(&hw_lowPowerCPUIO_device);
#ifdef DEBUG
       printk("hw_lowPowerCPUIO_device init..\n");	
#endif
	return 0;
}

static void __exit hw_lowPowerCPUIO_exit(void)
{
	platform_device_unregister(&hw_lowPowerCPUIO_device);
#ifdef DEBUG
       printk("hw_lowPowerCPUIO_device init..\n");	
#endif

}

module_init(hw_lowPowerCPUIO_init);
module_exit(hw_lowPowerCPUIO_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("xzz");


