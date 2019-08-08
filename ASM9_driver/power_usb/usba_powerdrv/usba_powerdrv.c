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
#define DEBUG

static char status[3];
struct usba_POWER_GPIO_definit
{
	int usba_POWER_group;
	int usba_POWER_gpio_pin;
};

static struct usba_POWER_GPIO_definit *plat_usba_POWER_gpio;
/**********************************led1 show store***************************************************/
static ssize_t usba_POWER_show(struct kobject *kobj,struct kobj_attribute *attr, char *buf)
{
#ifdef DEBUG	
	printk("enter usba_POWER_show\n");
#endif	   
 	return sprintf(buf, "%s\n", status);
}
 
static ssize_t usba_POWER_store(struct kobject *kobj,struct kobj_attribute *attr, const char *buf, size_t count)
{
#ifdef DEBUG
	printk("usba_POWER write: %s\n",buf);
#endif
	if(strcmp(buf,"1")==true){
		write_GPIO(plat_usba_POWER_gpio->usba_POWER_group,plat_usba_POWER_gpio->usba_POWER_gpio_pin,1);	
		strcpy(status,"1");
	}
	else
	{
		write_GPIO(plat_usba_POWER_gpio->usba_POWER_group,plat_usba_POWER_gpio->usba_POWER_gpio_pin,0);	
		strcpy(status,"0");
	}
	
	return count;
}
/**********************************led2 show store***************************************************/
static struct kobj_attribute usba_POWER_attr = __ATTR(usba_POWER, 0644,usba_POWER_show, usba_POWER_store);
static struct attribute *usba_POWER_attribute[] = {
	&usba_POWER_attr.attr,
	NULL,

};

static struct attribute_group usba_POWER_group = {
	.attrs = usba_POWER_attribute,

};

static struct kobject *usba_POWER_kobj = NULL;
static int __devinit usba_POWER_probe(struct platform_device *pdev)
{
	int ret = 0;
	plat_usba_POWER_gpio = pdev->dev.platform_data;

#ifdef DEBUG
	printk("usba_POWER gpio_group = %d\n",plat_usba_POWER_gpio->usba_POWER_group);
	printk("usba_POWER gpio_pin = %d\n",plat_usba_POWER_gpio->usba_POWER_gpio_pin);
#endif
	usba_POWER_kobj = kobject_create_and_add("Cusba_POWER", usba_POWER_kobj);//在/sys/目录下创建一个usba_POWER目录
	if (!usba_POWER_kobj)
		return -ENOMEM;
	ret = sysfs_create_group(usba_POWER_kobj, &usba_POWER_group);
	if (ret)
		kobject_put(usba_POWER_kobj);
	set_pin_mux(plat_usba_POWER_gpio->usba_POWER_group,plat_usba_POWER_gpio->usba_POWER_gpio_pin,0);//usba_POWER
#ifdef DEBUG
	printk("usba_POWER probe...\n");
#endif
	return 0;
}



static int usba_POWER_remove(struct platform_device *pdev)
{
	sysfs_remove_group(usba_POWER_kobj, &usba_POWER_group);
	kobject_del(usba_POWER_kobj);
        clear_GPIO(plat_usba_POWER_gpio->usba_POWER_group,plat_usba_POWER_gpio->usba_POWER_gpio_pin);
	printk("usba_POWER_remove.. \n");
	return 0;
}

static struct platform_driver usba_POWER_platform_driver = {
	.probe= usba_POWER_probe,
	.remove= usba_POWER_remove,
	.driver= {
	       .name= "usba_POWER_driver_device",
	       .owner= THIS_MODULE,

	},


};




static int kobj_usba_POWER_init(void)
{
	platform_driver_register(&usba_POWER_platform_driver);
#ifdef DEBUG
	printk("kobj usba_POWER init...\n");
#endif

	return 0;
}
 
static int  kobj_usba_POWER_exit(void)
{
	platform_driver_unregister(&usba_POWER_platform_driver);
#ifdef DEBUG
	printk("kobj usba_POWER exit...\n");
#endif
	return 0;
}
 
module_init(kobj_usba_POWER_init);
module_exit(kobj_usba_POWER_exit);
