#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/platform_device.h>


struct rx8025_GPIO_definit
{
	int rx8025_IO_GPIOgroup; //RX8025 data
	int rx8025_IO_pin; //RX8025 data
	int rx8025_SCLK_GPIOgroup; //RX8025 data sclk
	int rx8025_SCLK_pin; //RX8025 data sclk
};

static struct rx8025_GPIO_definit rx8025dev_data = 
{
	.rx8025_IO_GPIOgroup = 4,
	.rx8025_IO_pin = 5,
	.rx8025_SCLK_GPIOgroup = 4,
	.rx8025_SCLK_pin = 6,
};

static void RX8025_device_release(struct device *dev)
{

}

static struct platform_device RX8025_device = 
{
	.name = "RX8025_device_driver",
	.id = -1,
	.dev = {
		.platform_data = &rx8025dev_data,
		.release = RX8025_device_release,
	},
};


static int __init RX8025_dev_init(void)
{
	platform_device_register(&RX8025_device);
	printk("RX8025_device_init..\n");
	return 0;
}

static void __exit RX8025_dev_exit(void)
{
	platform_device_unregister(&RX8025_device);
	printk("RX8025_device_exit..\n");	
}

module_init(RX8025_dev_init);
module_exit(RX8025_dev_exit);
MODULE_LICENSE("Dual BSD/GPL"); 
MODULE_AUTHOR("xzz");
MODULE_DESCRIPTION("RX8025_device");
