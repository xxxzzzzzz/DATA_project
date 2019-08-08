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
#include <linux/i2c.h>
#include <linux/mutex.h>
#include <linux/kobject.h>
#include <linux/sysfs.h>
#include <linux/string.h>
#include <linux/stat.h>
#include "RX8025SA_rw.h"

//#define DEBUG


static struct rx8025_GPIO_definit *plat_gpio_data;


static int RX8025_rtc_open(struct device *dev){

#ifdef DEBUG       
	printk("enter RX8025 open\n");
#endif
	return 0;
}

static int RX8025_rtc_read_time(struct device *dev, struct rtc_time *tm){

#ifdef DEBUG       
	printk("RX8025_rtc_read_time\n");
#endif
	RX8025_get_Time(tm,plat_gpio_data);

	return rtc_valid_tm(tm);
}

static int RX8025_rtc_set_time(struct device *dev, struct rtc_time *tm){

#ifdef DEBUG      
	printk("RX8025_rtc_set_time\n");
	printk("set_time sec=%d min=%d hour=%d mday=%d tm_mon=%d tm_year=%d\n",tm->tm_sec,tm->tm_min,tm->tm_hour,tm->tm_mday,tm->tm_mon,tm->tm_year);
#endif
	RX8025_Time_set(tm,plat_gpio_data);

	return 0;
}

static int RX8025_rtc_getalarm(struct device *dev, struct rtc_wkalrm *alrm)
{
#ifdef DEBUG
	printk(" RX8025_rtc_getalarm\n");	
#endif
	return 0;
}

static int RX8025_rtc_setalarm(struct device *dev, struct rtc_wkalrm *alrm)
{
	struct rtc_time *tm = &alrm->time;
#ifdef DEBUG	
	printk("rtc_set_alarm sec=%d min=%d hour=%d mday=%d tm_mon=%d tm_year=%d\n",tm->tm_sec,tm->tm_min,tm->tm_hour,tm->tm_mday,tm->tm_mon,tm->tm_year);
	printk(" RX8025_rtc_setalarm\n");	
#endif

	RX8025_INTA_OutIrq_set((unsigned char)tm->tm_min,(unsigned char)tm->tm_hour,plat_gpio_data);

	return 0;
}


static int RX8025_rtc_ioctl(struct device *dev, unsigned int cmd,unsigned long arg){
	struct rtc_time tm;
	struct rtc_wkalrm alrm;
	void __user *argp = (void __user *)arg;
	switch (cmd) {
				
		case RTC_SET_TIME:
			if(copy_from_user(&tm,argp,sizeof(struct rtc_time))!=0)
				return -EFAULT;
			RX8025_rtc_set_time(dev, &tm);
			break;

		case RTC_RD_TIME:
			RX8025_rtc_read_time(dev, &tm);
			if(copy_to_user(argp,&tm,sizeof(struct rtc_time))!=0)
				return -EFAULT;
			break;

		case RTC_ALM_SET:
			if(copy_from_user(&alrm.time,argp,sizeof(struct rtc_time))!=0)
				return -EFAULT;
			RX8025_rtc_setalarm(dev,&alrm);
			break;

		case RTC_ALM_READ:
			RX8025_rtc_getalarm(dev, &alrm);
			if(copy_to_user(argp,&alrm.time,sizeof(struct rtc_time))!=0)
				return -EFAULT;
			break;
		default:
			return -ENOIOCTLCMD;
	}
	printk(" enter RX8025 IOCTL\n");
        return 0;
}

static void RX8025_rtc_release(struct device *dev)
{

       printk("close RX8025 rtc_release\n");

}



static struct rtc_class_ops RX8025_rtc_ops = {
	.open =RX8025_rtc_open,
	.read_time= RX8025_rtc_read_time,
	.set_time= RX8025_rtc_set_time,
	.read_alarm= RX8025_rtc_getalarm,
	.set_alarm= RX8025_rtc_setalarm,
	.ioctl= RX8025_rtc_ioctl,
	.release = RX8025_rtc_release,
};

static char CTFG_flag[1];
static ssize_t RX8025_get_show(struct kobject *kobj,struct kobj_attribute *attr,char *buf)
{
	int data = 0;

	data = RX8025_Read_CONTROL2_CTFG(plat_gpio_data);
	if(data == 1)
		strcpy(CTFG_flag,"1");
	else
		strcpy(CTFG_flag,"0");

	return sprintf(buf,"%s\n",CTFG_flag);
}


static ssize_t RX8025_set_store(struct kobject *kobj,struct kobj_attribute *attr,const char *buf,size_t count)
{
#ifdef DEBUG
	printk("store : %s\n",buf);
#endif
	if(!(strcmp(buf,"1") == true))
	{
		RX8025_SET_CONTROL2_CTFG(plat_gpio_data);
	}
	else
	{
	}
	return count;
}

static struct kobj_attribute RX8025sysfsdriver = __ATTR(RX8025_ctl_reg,0644,RX8025_get_show,RX8025_set_store);

static struct attribute *RX8025attr[] = {
	&RX8025sysfsdriver.attr,
	NULL,
};

static struct attribute_group RX8025_group = {
	.attrs = RX8025attr,
};


static struct kobject *RX8025_kobj = NULL;

static struct rtc_device *rtc;

static int __devinit RX8025_probe(struct platform_device *pdev){
	
	int ret = 0;

	plat_gpio_data = pdev->dev.platform_data;//把GPIO管脚号从设备驱动里面获取出来

#ifdef DEBUG 
	printk("enter probe\n");
	printk("rx8025_IO_GPIOgroup = %d\n",plat_gpio_data->rx8025_IO_GPIOgroup);
	printk("rx8025_IO_pin = %d\n",plat_gpio_data->rx8025_IO_pin);
	printk("rx8025_SCLK_GPIOgroup = %d\n",plat_gpio_data->rx8025_SCLK_GPIOgroup);
	printk("rx8025_SCLK_pin = %d\n",plat_gpio_data->rx8025_SCLK_pin);
#endif
	
	set_pin_mux(plat_gpio_data->rx8025_IO_GPIOgroup,plat_gpio_data->rx8025_IO_pin,0);//设置管脚为普通GPIO
	set_pin_mux(plat_gpio_data->rx8025_SCLK_GPIOgroup,plat_gpio_data->rx8025_SCLK_pin,0);//设置管脚为普通GPIO

	rtc = rtc_device_register("RX8025_rtc",&pdev->dev,&RX8025_rtc_ops,THIS_MODULE);
	if (IS_ERR(rtc))
		return PTR_ERR(rtc);

	platform_set_drvdata(pdev, rtc);

	RX8025_kobj = kobject_create_and_add("CdataRX8025_delay_out",RX8025_kobj);
	if(!RX8025_kobj)
		printk("RX8025_kobj create failed..\n");
	
	ret = sysfs_create_group(RX8025_kobj,&RX8025_group);
	

	printk("enter RX8025 RTC probe\n ");
	return 0;
}


static int RX8025_remove(struct platform_device *pdev){
	rtc_device_unregister(rtc);
	sysfs_remove_group(RX8025_kobj,&RX8025_group);
	kobject_put(RX8025_kobj);
	printk("RX8025 romove..\n");
	return 0;
}


static struct platform_driver RX8025_platform_driver = {
	.probe= RX8025_probe,
	.remove= RX8025_remove,
	.driver= {
		.name = "RX8025_device_driver",
		.owner= THIS_MODULE,
	},

};



static int __init RX8025_drv_init(void){
	
	platform_driver_register(&RX8025_platform_driver);
	printk("RX8025_driver init...\n");
	return 0;
}



static void __exit RX8025_drv_exit(void){
	
 	platform_driver_unregister(&RX8025_platform_driver);
	printk("RX8025_driver exit...\n");
}



module_init(RX8025_drv_init);
module_exit(RX8025_drv_exit);
MODULE_LICENSE("Dual BSD/GPL"); 
