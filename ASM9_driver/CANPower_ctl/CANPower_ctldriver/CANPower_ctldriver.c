#include <linux/device.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/string.h>
#include <linux/sysfs.h>
#include <linux/stat.h>
#include <linux/platform_device.h> 
#include <linux/kobject.h>
#include <linux/gpio.h>
#include <linux/io.h>
#include<mach/pincontrol.h>

MODULE_AUTHOR("xiang");
MODULE_LICENSE("Dual BSD/GPL");
//#define DEBUG

struct CANPower_Ctl_GPIO_definit
{
	int CANPower_Ctl_group;
	int CANPower_Ctl_gpio_pin;
};

static struct CANPower_Ctl_GPIO_definit *plat_io_gpio;
/**********************************CANPower_Ctl1 show store***************************************************/
static char CANPower_Ctl1_status[3];
static ssize_t CANPower_Ctl1_show(struct kobject *kobj,struct kobj_attribute *attr, char *buf)
{
#ifdef DEBUG	
	printk("enter CANPower_Ctl1_show\n");
#endif	   
 	return sprintf(buf, "%s\n", CANPower_Ctl1_status); 
}
 
static ssize_t CANPower_Ctl1_store(struct kobject *kobj,struct kobj_attribute *attr, const char *buf, size_t count)
{
#ifdef DEBUG
	printk("CANPower_Ctl1 write: %s\n",buf);
#endif
	if(strcmp(buf,"1")==true){
		write_GPIO(plat_io_gpio->CANPower_Ctl_group,plat_io_gpio->CANPower_Ctl_gpio_pin,1);	
		strcpy(CANPower_Ctl1_status,"1");
	}
	else
	{
		write_GPIO(plat_io_gpio->CANPower_Ctl_group,plat_io_gpio->CANPower_Ctl_gpio_pin,0);	
		strcpy(CANPower_Ctl1_status,"0");
	}
	
	return count;
}

static struct kobj_attribute CANPower_Ctl1_attr = __ATTR(CANpowerCtl, 0644,CANPower_Ctl1_show,CANPower_Ctl1_store);
static struct attribute *CANPower_Ctl_attribute[] = {
	&CANPower_Ctl1_attr.attr,
	NULL,

};

static struct attribute_group CANPower_Ctl_group = {
	.attrs = CANPower_Ctl_attribute,

};

static struct kobject *CANPower_Ctl_kobj = NULL;
static int __devinit CANPower_Ctl_probe(struct platform_device *pdev)
{
	int ret = 0;
	plat_io_gpio = pdev->dev.platform_data;

#ifdef DEBUG
	printk("CANPower_Ctl1 gpio_group = %d\n",plat_io_gpio->hw_lowPowerCPUIO_group);
	printk("CANPower_Ctl1 gpio_pin = %d\n",plat_io_gpio->hw_lowPowerCPUIO_gpio_pin);

#endif
	CANPower_Ctl_kobj = kobject_create_and_add("CANpowerCtl", CANPower_Ctl_kobj);//在/sys/目录下创建一个led目录
	if (!CANPower_Ctl_kobj)
		return -ENOMEM;
	ret = sysfs_create_group(CANPower_Ctl_kobj, &CANPower_Ctl_group);
	if (ret)
		kobject_put(CANPower_Ctl_kobj);

	set_pin_mux(plat_io_gpio->CANPower_Ctl_group,plat_io_gpio->CANPower_Ctl_gpio_pin,0);//led1
#ifdef DEBUG
	printk("CANPower_Ctl probe...\n");
#endif
	return 0;
}



static int CANPower_Ctl_remove(struct platform_device *pdev)
{
	sysfs_remove_group(CANPower_Ctl_kobj, &CANPower_Ctl_group);
	kobject_del(CANPower_Ctl_kobj);
        clear_GPIO(plat_io_gpio->CANPower_Ctl_group,plat_io_gpio->CANPower_Ctl_gpio_pin);
	printk("CANPower_Ctl_remove.. \n");
	return 0;
}

static struct platform_driver CANPower_Ctl_platform_driver = {
	.probe= CANPower_Ctl_probe,
	.remove= CANPower_Ctl_remove,
	.driver= {
	       .name= "CAN_PowerCtl_driver_device",
	       .owner= THIS_MODULE,

	},

};




static int kobj_CANPower_Ctl_init(void)
{
	platform_driver_register(&CANPower_Ctl_platform_driver);
#ifdef DEBUG
	printk("kobj CANPower_Ctl init...\n");
#endif

	return 0;
}
 
static void  kobj_CANPower_Ctl_exit(void)
{
	platform_driver_unregister(&CANPower_Ctl_platform_driver);
#ifdef DEBUG
	printk("kobj CANPower_Ctl exit...\n");
#endif
}
 
module_init(kobj_CANPower_Ctl_init);
module_exit(kobj_CANPower_Ctl_exit);
