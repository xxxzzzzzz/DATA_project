#include<linux/init.h>
#include<linux/module.h>
#include<linux/platform_device.h>

#define DEBUG


struct Cdata_1hm_GPIO_definit
{
	int weight1_CLK_GPIOgroup; //pin in where group
	int weight1_CLK_pin; //pin group to single pin
	int weight1_data_GPIOgroup; //pin in where group
	int weight1_data_pin; //pin group to single pin
};

static struct Cdata_1hm_GPIO_definit Cdata_1hm_data = 
{
	.weight1_CLK_GPIOgroup = 0,
	.weight1_CLK_pin = 4,
	.weight1_data_GPIOgroup = 0,
	.weight1_data_pin = 1,
};

void Cdata_1hm_release(struct device *dev)
{

}

struct platform_device Cdata_1hm_device = 
{
	.name = "1hm_driver_device",
	.id = -1,
		.dev = {
			.platform_data = &Cdata_1hm_data,
			.release = Cdata_1hm_release,
		},
};

static int __init Cdata_1hm_device_init(void)
{
	platform_device_register(&Cdata_1hm_device);
#ifdef DEBUG
       printk("Cdata_1hm_device init..\n");	
#endif
	return 0;
}

static void __exit Cdata_1hm_device_exit(void)
{
	platform_device_unregister(&Cdata_1hm_device);
#ifdef DEBUG
       printk("Cdata_1hm_device init..\n");	
#endif

}

module_init(Cdata_1hm_device_init);
module_exit(Cdata_1hm_device_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("xzz");


