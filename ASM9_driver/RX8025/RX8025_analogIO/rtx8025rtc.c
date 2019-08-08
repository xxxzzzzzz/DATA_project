#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
//#include <linux/device.h> 
#include <linux/platform_device.h>
#include <linux/rtc.h>
#include <linux/io.h>
#include <linux/gpio.h>
#include <linux/uaccess.h>

#include <linux/delay.h>
/*三个字符设备函数*/
#include <linux/fs.h>
/*MKDEV转换设备号数据类型的宏定义*/
#include <linux/kdev_t.h>
/*定义字符设备的结构体*/
#include <linux/cdev.h>
/*分配内存空间函数头文件*/
#include <linux/slab.h>
#include<mach/pincontrol.h>
#include "RX8025_rw.h"

MODULE_LICENSE("Dual BSD/GPL"); 

#define DEBUG


/*
 *GPIO管脚号定义
 *GPIO4_7 -->SDA
 *GPIO4_6 -->SCL 
 */
static struct rx8025_GPIO_definit rx8025dev_data = 
{
	.rx8025_IO_GPIOgroup = 4,
	.rx8025_IO_pin = 5,
	.rx8025_SCLK_GPIOgroup = 4,
	.rx8025_SCLK_pin = 6,
};


static struct rx8025_GPIO_definit *plat_gpio_data;

static void rx8025_rtc_release(struct device *dev){
        clear_GPIO(plat_gpio_data->rx8025_IO_GPIOgroup,plat_gpio_data->rx8025_IO_pin);
        clear_GPIO(plat_gpio_data->rx8025_SCLK_GPIOgroup,plat_gpio_data->rx8025_SCLK_pin);

        printk("close rx8025 rtc_release\n");
}




static int __devinit rx8025_probe(struct platform_device *pdev){
	
	plat_gpio_data = pdev->dev.platform_data;//把GPIO管脚号从设备驱动里面获取出来
	unsigned char i;
	//struct rtc_time tm;
	struct RX8025_Time settm;
	struct RX8025_Time gettm;

	printk("enter rx8025 RTC probe\n ");
/*DEBUG 查看获取的GPIO管脚号是否正确*/
#ifdef DEBUG 
	printk("enter probe\n");
	printk("rx8025_IO_GPIOgroup = %d\n",plat_gpio_data->rx8025_IO_GPIOgroup);
	printk("rx8025_IO_pin = %d\n",plat_gpio_data->rx8025_IO_pin);
	printk("rx8025_SCLK_GPIOgroup = %d\n",plat_gpio_data->rx8025_SCLK_GPIOgroup);
	printk("rx8025_SCLK_pin = %d\n",plat_gpio_data->rx8025_SCLK_pin);
#endif
	
	set_pin_mux(plat_gpio_data->rx8025_IO_GPIOgroup,plat_gpio_data->rx8025_IO_pin,0);//设置管脚为普通GPIO
	set_pin_mux(plat_gpio_data->rx8025_SCLK_GPIOgroup,plat_gpio_data->rx8025_SCLK_pin,0);//设置管脚为普通GPIO
	
	//RX8025_regTime_test(plat_gpio_data);//测试GPIO模拟I2C协议
	
	settm.sec = 3;
			
	settm.min = 10;
	settm.hours = 1;
	settm.week = 0;
	settm.day = 0;
	settm.month = 0;
	settm.year = 0;

	RX8025_Time_set(&settm,plat_gpio_data);
	
//	RX8025_INTA_REG_CONTROL2_get_set_CTFG(plat_gpio_data);
//	set_INTA_out_Mode(0x02,plat_gpio_data);
//	 RX8025_INTA_OutIrq_set(&settm,2,1,plat_gpio_data);

	for(i=0;i<10;i++)
	{
		RX8025_get_Time(&gettm,plat_gpio_data);

		printk("sec = %d min = %d hour = %d mday = %d mon = %d year = %d\n",(gettm.sec&0x7f),gettm.min,gettm.hours,gettm.day,gettm.month,gettm.year);
		mdelay(1000);

	}

	
	
	return 0;
}


static int rx8025_remove(struct platform_device *pdev){
#ifdef DEBUG
	printk("rx8025 romove..\n");
#endif
	return 0;
}


static struct platform_driver rx8025_platform_driver = {
	.probe= rx8025_probe,
	.remove= rx8025_remove,
	.driver= {
	       .name= "RX8025_driver_device",
	       .owner= THIS_MODULE,

	},
};

void rx8025_release(struct device *dev)
{

}

struct platform_device rx8025_device = 
{
	.name = "RX8025_driver_device",
	.id = -1,
		.dev = {
			.platform_data = &rx8025dev_data,
			.release = rx8025_release,
		},
};


static int __init rx8025_rtc_init(void){
	platform_driver_register(&rx8025_platform_driver);
	platform_device_register(&rx8025_device);
	printk("rx8025_driver init...\n");
	return 0;
}

static int __exit rx8025_rtc_exit(void){
 	platform_driver_unregister(&rx8025_platform_driver);
	platform_device_unregister(&rx8025_device);
	printk("rx8025_driver exit...\n");
       return 0;
}

module_init(rx8025_rtc_init);
module_exit(rx8025_rtc_exit);
