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




static int RX8025_rtc_open(struct device *dev){
       
//	printk("enter RX8025 open\n");
	return 0;
}

static int RX8025_rtc_read_time(struct device *dev, struct rtc_time *tm){
       
//	printk("RX8025_rtc_read_time\n");
	tm->tm_sec = 10;
	tm->tm_min = 10;
	tm->tm_hour = 10;
	tm->tm_mday = 10;
	tm->tm_mon = 10;
	tm->tm_year = 10;
	return rtc_valid_tm(tm);
}

static int RX8025_rtc_set_time(struct device *dev, struct rtc_time *tm){
      
//	printk("RX8025_rtc_set_time\n");
	printk("set_time sec=%d min=%d hour=%d mday=%d tm_mon=%d tm_year=%d\n",tm->tm_sec,tm->tm_min,tm->tm_hour,tm->tm_mday,tm->tm_mon,tm->tm_year);
	return 0;
}

static int RX8025_rtc_getalarm(struct device *dev, struct rtc_wkalrm *alrm)
{
	struct rtc_time *alm_tm = &alrm->time;
	alm_tm->tm_sec  = 20;
	alm_tm->tm_min  = 20;
	alm_tm->tm_hour = 20;
	alm_tm->tm_mon  = 20;
	alm_tm->tm_mday = 20;
	alm_tm->tm_year = 20;
//	printk(" RX8025_rtc_getalarm\n");	
	return 0;
}

static int RX8025_rtc_setalarm(struct device *dev, struct rtc_wkalrm *alrm)
{
	struct rtc_time *tm = &alrm->time;
	
printk("rtc_set_alarm sec=%d min=%d hour=%d mday=%d tm_mon=%d tm_year=%d\n",tm->tm_sec,tm->tm_min,tm->tm_hour,tm->tm_mday,tm->tm_mon,tm->tm_year);
//	printk(" RX8025_rtc_setalarm\n");	
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

		case RTC_ALM_SET:
			printk("RTC_ALM_SET........\n");
			if(copy_from_user(&alrm.time,argp,sizeof(struct rtc_time))!=0)
				return -EFAULT;
			RX8025_rtc_setalarm(dev,&alrm);
			break;

		case RTC_ALM_READ:
			RX8025_rtc_getalarm(dev, &alrm);
			if(copy_to_user(argp,&alrm.time,sizeof(struct rtc_time))!=0)
				return -EFAULT;

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



static int __devinit RX8025_probe(struct platform_device *pdev){
	
	struct rtc_device *rtc;
	rtc = rtc_device_register("RX8025_rtc",&pdev->dev,&RX8025_rtc_ops,THIS_MODULE);
	if (IS_ERR(rtc))
		return PTR_ERR(rtc);
	platform_set_drvdata(pdev, rtc);
	printk("enter RX8025 RTC probe\n ");
	return 0;
}


static int RX8025_remove(struct platform_device *pdev){
	printk("RX8025 romove..\n");
	return 0;
}


static struct platform_driver RX8025_platform_driver = {
	.probe= RX8025_probe,
	.remove= RX8025_remove,
	.driver= {
	       .name= "RX8025_driver_device",
	       .owner= THIS_MODULE,
	},

};


void RX8025_release(struct device *dev)
{

}

struct platform_device RX8025_device = 
{
	.name = "RX8025_driver_device",
	.id = -1,
		.dev = {
			.release = RX8025_release,
		},
};


static int __init RX8025_rtc_init(void){
	
	platform_device_register(&RX8025_device);
	platform_driver_register(&RX8025_platform_driver);
	printk("RX8025_driver init...\n");
	return 0;
}



static void __exit RX8025_rtc_exit(void){
	
	platform_device_unregister(&RX8025_device);
 	platform_driver_unregister(&RX8025_platform_driver);
	printk("RX8025_driver exit...\n");
}



module_init(RX8025_rtc_init);
module_exit(RX8025_rtc_exit);
MODULE_LICENSE("Dual BSD/GPL"); 
