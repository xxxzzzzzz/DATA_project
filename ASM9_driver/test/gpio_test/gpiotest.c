#include<linux/module.h>
#include<linux/init.h>
#include<linux/gpio.h>//gpio_request，gpio_direction_output，gpio_set_value都需要这个头文件
#include<linux/delay.h>
#include<mach/pincontrol.h>

static int __init asm9260t_gpiotest_init(void) 
{
	int i = 0,z = 0;
	printk("init asm9260t gpio module_driver\n");
	set_pin_mux(17, 4, 0);//使用某个GPIO要先设置管脚
	for(i=0;i<5;i++)
	{
		write_GPIO(17,4,0);//设置管脚输出电平
		mdelay(1000);
		write_GPIO(17,4,1);
		mdelay(1000);
	}
	set_pin_mux(0,1,0);
	for(i=0;i<5;i++)
	{
		while(1){
		z = read_GPIO(0,1);
		printk("gpio input data = %d\n",z);
		mdelay(500);
		}
	}


	return 0;

}

static void __exit asm9260t_gpiotest_exit(void) 
{

	printk("exit  asm9260t gpio  module_driver\n");

}	
module_init(asm9260t_gpiotest_init);
module_exit(asm9260t_gpiotest_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("xzz");
MODULE_DESCRIPTION("asm9260t_gpio_test"); 
