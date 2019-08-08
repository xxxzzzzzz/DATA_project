#include<linux/init.h>
#include<linux/module.h>
#include<linux/platform_device.h>

#define DEBUG


struct CAN_PowerCtl_GPIO_definit
{
	int CAN_PowerCtl1_group;
	int CAN_PowerCtl1_gpio_pin;
};

static struct CAN_PowerCtl_GPIO_definit CAN_PowerCtl_data = 
{
	.CAN_PowerCtl1_group = 4,
	.CAN_PowerCtl1_gpio_pin = 1,
};

void CAN_PowerCtl_release(struct device *dev)
{

}

struct platform_device CAN_PowerCtl_device = 
{
	.name = "CAN_PowerCtl_driver_device",
	.id = -1,
		.dev = {
			.platform_data = &CAN_PowerCtl_data,
			.release = CAN_PowerCtl_release,
		},
};

static int __init CAN_PowerCtl_init(void)
{
	platform_device_register(&CAN_PowerCtl_device);
#ifdef DEBUG
       printk("CAN_PowerCtl_device init..\n");	
#endif
	return 0;
}

static void __exit CAN_PowerCtl_exit(void)
{
	platform_device_unregister(&CAN_PowerCtl_device);
#ifdef DEBUG
       printk("CAN_PowerCtl_device init..\n");	
#endif

}

module_init(CAN_PowerCtl_init);
module_exit(CAN_PowerCtl_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("xzz");


