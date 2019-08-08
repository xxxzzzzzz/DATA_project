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
#include "ds1302_rw.h"

MODULE_LICENSE("Dual BSD/GPL"); 

//#define ds1302_printk_switch
//#define DEBUG

static struct Cdata_DS1302_GPIO_definit *plat_gpio_data;

static atomic_t ds1302_atomic = ATOMIC_INIT(1); 

static int ds1302_rtc_open(struct device *dev){

	
	if(!atomic_dec_and_test(&ds1302_atomic)){
     	atomic_inc(&ds1302_atomic);
	return -EBUSY;
	}
       
	set_pin_mux(plat_gpio_data->ds1302_IO_GPIOgroup,plat_gpio_data->ds1302_IO_pin,0);
	set_pin_mux(plat_gpio_data->ds1302_SCLK_GPIOgroup,plat_gpio_data->ds1302_SCLK_pin,0);
	set_pin_mux(plat_gpio_data->ds1302_CE_GPIOgroup,plat_gpio_data->ds1302_CE_pin,0);
	printk("enter ds1302 open\n");

	return 0;
}

static int ds1302_rtc_read_time(struct device *dev, struct rtc_time *tm){
       
	static unsigned int sec,min,hour,day,month,year;
	static unsigned int sec_unit,min_unit,hour_unit,day_unit,month_unit,year_unit;
	static unsigned int sec_shi,min_shi,hour_shi,day_shi,month_shi,year_shi;
#ifdef ds1302_printk_switch
      printk("ds1302_rtc_read_time\n");
#endif
      sec=read_DS1302(READ_SECOND,plat_gpio_data);
      min=read_DS1302(READ_MINUTE,plat_gpio_data);
      hour=read_DS1302(READ_HOUR,plat_gpio_data);
      day=read_DS1302(READ_DAY,plat_gpio_data);
      month=read_DS1302(READ_MONTH,plat_gpio_data);
      year=read_DS1302(READ_YEARS,plat_gpio_data);
      /*sec*/
      sec_unit=sec&0x0f;
      sec_shi =sec>>4;
      sec_shi=sec_shi*10;
      sec=sec_shi+sec_unit; 
      tm->tm_sec=sec;
      /*min*/
      min_unit=min&0x0f;
      min_shi =min>>4;
      min_shi =min_shi*10;
      min=min_shi+min_unit;
      tm->tm_min=min;
      /*hour*/
      hour_unit=hour&0x0f;
      hour_shi =hour>>4;
      hour_shi =hour_shi*10;
      hour=hour_shi+hour_unit;
      tm->tm_hour=hour;
      /*day*/
      day_unit=day&0x0f;
      day_shi =day>>4;
      day_shi =day_shi*10;
      day = day_shi+day_unit;
      tm->tm_mday=day;
      /*month*/
      month_unit=month&0x0f;
      month_shi =month>>4;
      month_shi =month_shi*10;
      month=month_shi+month_unit;
      tm->tm_mon=month-1;
      /*year*/
      year_unit=year&0x0f;
      year_shi =year>>4;
      year_shi =year_shi*10;
      year=year_shi+year_unit;
      tm->tm_year=year+2000;
	if (tm->tm_year < 70)
	  tm->tm_year += 100;
#ifdef ds1302_printk_switch
     printk(" kernel read sec=%d min=%d hour=%d mday=%d tm_mon=%d tm_year=%d\n",tm->tm_sec,tm->tm_min,tm->tm_hour,tm->tm_mday,tm->tm_mon,tm->tm_year);
#endif    
    //  tm->tm_mon=11;	
     return rtc_valid_tm(tm);

}

static int ds1302_rtc_set_time(struct device *dev, struct rtc_time *tm){
	static int year;
      	static unsigned char bcd8421[60]={0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x10,0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x20,0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,0x30,0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x40,0x41,0x42,0x43,0x44,0x45,0x46,0x47,0x48,0x49,0x50,0x51,0x52,0x53,0x54,0x55,0x56,0x57,0x58,0x59};
#ifdef ds1302_printk_switch
	printk("enter ds1302_rtc_set_time\n");
	printk("set_time sec=%d min=%d hour=%d mday=%d tm_mon=%d tm_year=%d\n",tm->tm_sec,tm->tm_min,tm->tm_hour,tm->tm_mday,tm->tm_mon,tm->tm_year);
#endif
      year= tm->tm_year%2000; 

      
#ifdef ds1302_printk_switch
	printk(" kernel setting sec=%d min=%d hour=%d mday=%d tm_mon=%d tm_year=%d\n",tm->tm_sec,tm->tm_min,tm->tm_hour,tm->tm_mday,tm->tm_mon,year);
#endif    
	init_ds1302(bcd8421[tm->tm_sec],bcd8421[tm->tm_min],bcd8421[tm->tm_hour],bcd8421[tm->tm_mday],bcd8421[tm->tm_mon],bcd8421[year],plat_gpio_data);
       
	return 0;
}

static int ds1302_rtc_ioctl(struct device *dev, unsigned int cmd,unsigned long arg){
	struct rtc_time tm;
	void __user *argp = (void __user *)arg;
	switch (cmd) {
				
		case RTC_SET_TIME:
			if(copy_from_user(&tm,argp,sizeof(struct rtc_time))!=0)
				return -EFAULT;
			ds1302_rtc_set_time(dev, &tm);
			break;
		case RTC_RD_TIME:
			ds1302_rtc_read_time(dev, &tm);
			if(copy_to_user(argp,&tm,sizeof(struct rtc_time))!=0)
				return -EFAULT;
			break;
		default:
			return -ENOIOCTLCMD;
	}
	printk(" enter rtc IOCTL\n");
        return 0;
}

static void ds1302_rtc_release(struct device *dev){
        clear_GPIO(plat_gpio_data->ds1302_IO_GPIOgroup,plat_gpio_data->ds1302_IO_pin);
        clear_GPIO(plat_gpio_data->ds1302_SCLK_GPIOgroup,plat_gpio_data->ds1302_SCLK_pin);
        clear_GPIO(plat_gpio_data->ds1302_CE_GPIOgroup,plat_gpio_data->ds1302_CE_pin);
	atomic_inc(&ds1302_atomic);
        printk("close ds1302 rtc_release\n");

}
static struct rtc_class_ops ds1302_rtc_ops = {
	.open =ds1302_rtc_open,
	.read_time= ds1302_rtc_read_time,
	.set_time= ds1302_rtc_set_time,
	.ioctl= ds1302_rtc_ioctl,
	.release = ds1302_rtc_release,
};



static int __devinit ds1302_probe(struct platform_device *pdev){

	
	struct rtc_device *rtc;
	plat_gpio_data = pdev->dev.platform_data;
        
#ifdef DEBUG
	printk("enter probe\n");
	printk("ds1302_IO_GPIOgroup = %d\n",plat_gpio_data->ds1302_IO_GPIOgroup);
	printk("ds1302_IO_pin = %d\n",plat_gpio_data->ds1302_IO_pin);
	printk("ds1302_SCLK_GPIOgroup = %d\n",plat_gpio_data->ds1302_SCLK_GPIOgroup);
	printk("ds1302_SCLK_pin = %d\n",plat_gpio_data->ds1302_SCLK_pin);
	printk("ds1302_CE_GPIOgroup = %d\n",plat_gpio_data->ds1302_CE_GPIOgroup);
	printk("ds1302_CE_pin = %d\n",plat_gpio_data->ds1302_CE_pin);
#endif
	rtc = rtc_device_register("ds1302_rtc",&pdev->dev,&ds1302_rtc_ops,THIS_MODULE);
	if (IS_ERR(rtc))
		return PTR_ERR(rtc);
	platform_set_drvdata(pdev, rtc);
	printk("enter ds1302 RTC probe\n ");
	return 0;
}


static int ds1302_remove(struct platform_device *pdev){
#ifdef DEBUG
	printk("ds1302 romove..\n");
#endif
	return 0;
}


static struct platform_driver ds1302_platform_driver = {
	.probe= ds1302_probe,
	.remove= ds1302_remove,
	.driver= {
	       .name= "ds1302_driver_device",
	       .owner= THIS_MODULE,

	},


};

static int __init ds1302_rtc_init(void){
	platform_driver_register(&ds1302_platform_driver);
	printk("ds1302_driver init...\n");
	return 0;
}



static int __exit ds1302_rtc_exit(void){
 	platform_driver_unregister(&ds1302_platform_driver);
	printk("ds1302_driver exit...\n");
       return 0;
}



module_init(ds1302_rtc_init);
module_exit(ds1302_rtc_exit);
