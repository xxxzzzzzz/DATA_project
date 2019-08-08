#include<linux/init.h>
#include<linux/module.h>
#include<linux/platform_device.h>

#define DEBUG


struct led_GPIO_definit
{
	int led1_group;
	int led1_gpio_pin;
	int led2_group;
	int led2_gpio_pin;
	int led3_group;
	int led3_gpio_pin;
	int led4_group;
	int led4_gpio_pin;
};

static struct led_GPIO_definit led_data = 
{
	.led1_group = 4,
	.led1_gpio_pin = 0,
	.led2_group = 4,
	.led2_gpio_pin = 1,
	.led3_group = 4,
	.led3_gpio_pin = 2,
	.led4_group = 2,
	.led4_gpio_pin = 1,
};

void led_release(struct device *dev)
{

}

struct platform_device led_device = 
{
	.name = "led_driver_device",
	.id = -1,
		.dev = {
			.platform_data = &led_data,
			.release = led_release,
		},
};

static int __init led_init(void)
{
	platform_device_register(&led_device);
#ifdef DEBUG
       printk("led_device init..\n");	
#endif
	return 0;
}

static void __exit led_exit(void)
{
	platform_device_unregister(&led_device);
#ifdef DEBUG
       printk("led_device init..\n");	
#endif

}

module_init(led_init);
module_exit(led_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("xzz");


