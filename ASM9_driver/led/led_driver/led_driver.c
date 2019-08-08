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

struct led_GPIO_definit
{
	int led1_group;
	int led1_gpio_pin;
	int led2_group;
	int led2_gpio_pin;
	int led3_group;
	int led3_gpio_pin;	
	int led4_group;
	int led4_gpio_pin;	
};

static struct led_GPIO_definit *plat_led_gpio;
/**********************************led1 show store***************************************************/
static char led1_status[3];
static ssize_t led1_show(struct kobject *kobj,struct kobj_attribute *attr, char *buf)
{
#ifdef DEBUG	
	printk("enter led1_show\n");
#endif	   
 	return sprintf(buf, "%s\n", led1_status); 
}
 
static ssize_t led1_store(struct kobject *kobj,struct kobj_attribute *attr, const char *buf, size_t count)
{
#ifdef DEBUG
	printk("led1 write: %s\n",buf);
#endif
	if(strcmp(buf,"1")==true){
		write_GPIO(plat_led_gpio->led1_group,plat_led_gpio->led1_gpio_pin,1);	
		strcpy(led1_status,"1");
	}
	else
	{
		write_GPIO(plat_led_gpio->led1_group,plat_led_gpio->led1_gpio_pin,0);	
		strcpy(led1_status,"0");
	}
	
	return count;
}
/**********************************led2 show store***************************************************/
static char led2_status[3];
static ssize_t led2_show(struct kobject *kobj,struct kobj_attribute *attr, char *buf)
{
#ifdef DEBUG	
	printk("enter led2_show\n");
#endif	   
 	return sprintf(buf, "%s\n", led2_status);
}
 
static ssize_t led2_store(struct kobject *kobj,struct kobj_attribute *attr, const char *buf, size_t count)
{
#ifdef DEBUG
	printk("led2 write: %s\n",buf);
#endif
	if(strcmp(buf,"1")==true){
		write_GPIO(plat_led_gpio->led2_group,plat_led_gpio->led2_gpio_pin,1);	
		strcpy(led2_status,"1");
	}
	else
	{
		write_GPIO(plat_led_gpio->led2_group,plat_led_gpio->led2_gpio_pin,0);	
		strcpy(led2_status,"0");
	}
	
	return count;
}
/*************************led3 show store***************************************************/
static char led3_status[3];
static ssize_t led3_show(struct kobject *kobj,struct kobj_attribute *attr, char *buf)
{
#ifdef DEBUG	
	printk("enter led3_show\n");
#endif	   
 	return sprintf(buf, "%s\n", led3_status);
}
 
static ssize_t led3_store(struct kobject *kobj,struct kobj_attribute *attr, const char *buf, size_t count)
{
#ifdef DEBUG
	printk("led3 write: %s\n",buf);
#endif
	if(strcmp(buf,"1")==true){
		write_GPIO(plat_led_gpio->led3_group,plat_led_gpio->led3_gpio_pin,1);	
		strcpy(led3_status,"1");
	}
	else
	{
		write_GPIO(plat_led_gpio->led3_group,plat_led_gpio->led3_gpio_pin,0);	
		strcpy(led3_status,"0");
	}
	
	return count;
}

/*************************led4 show store***************************************************/
static char led4_status[3];
static ssize_t led4_show(struct kobject *kobj,struct kobj_attribute *attr, char *buf)
{
#ifdef DEBUG	
	printk("enter led4_show\n");
#endif	   
 	return sprintf(buf, "%s\n", led4_status);
}

static ssize_t led4_store(struct kobject *kobj,struct kobj_attribute *attr, const char *buf, size_t count)
{
#ifdef DEBUG
	printk("led4 write: %s\n",buf);
#endif
	if(strcmp(buf,"1")==true){
		write_GPIO(plat_led_gpio->led4_group,plat_led_gpio->led4_gpio_pin,1);	
		strcpy(led4_status,"1");
	}
	else
	{
		write_GPIO(plat_led_gpio->led4_group,plat_led_gpio->led4_gpio_pin,0);	
		strcpy(led4_status,"0");
	}
	
	return count;
}


static struct kobj_attribute led1_attr = __ATTR(led1, 0644,led1_show, led1_store);
static struct kobj_attribute led2_attr = __ATTR(led2, 0644,led2_show, led2_store);
static struct kobj_attribute led3_attr = __ATTR(led3, 0644,led3_show, led3_store);
static struct kobj_attribute led4_attr = __ATTR(led4, 0644,led4_show, led4_store);
static struct attribute *led_attribute[] = {
	&led1_attr.attr,
	&led2_attr.attr,
	&led3_attr.attr,
	&led4_attr.attr,
	NULL,

};

static struct attribute_group led_group = {
	.attrs = led_attribute,

};

static struct kobject *led_kobj = NULL;
static int __devinit led_probe(struct platform_device *pdev)
{
	int ret = 0;
	plat_led_gpio = pdev->dev.platform_data;

#ifdef DEBUG
	printk("led1 gpio_group = %d\n",plat_led_gpio->led1_group);
	printk("led1 gpio_pin = %d\n",plat_led_gpio->led1_gpio_pin);
	printk("led2 gpio_group = %d\n",plat_led_gpio->led2_group);
	printk("led2 gpio_pin = %d\n",plat_led_gpio->led2_gpio_pin);
	printk("led3 gpio_group = %d\n",plat_led_gpio->led3_group);
	printk("led3 gpio_pin = %d\n",plat_led_gpio->led3_gpio_pin);
	printk("led4 gpio_group = %d\n",plat_led_gpio->led4_group);
	printk("led4 gpio_pin = %d\n",plat_led_gpio->led4_gpio_pin);

#endif
	led_kobj = kobject_create_and_add("Cled", led_kobj);//在/sys/目录下创建一个led目录
	if (!led_kobj)
		return -ENOMEM;
	ret = sysfs_create_group(led_kobj, &led_group);
	if (ret)
		kobject_put(led_kobj);
	set_pin_mux(plat_led_gpio->led1_group,plat_led_gpio->led1_gpio_pin,0);//led1
	set_pin_mux(plat_led_gpio->led2_group,plat_led_gpio->led2_gpio_pin,0);//led2	
	set_pin_mux(plat_led_gpio->led3_group,plat_led_gpio->led3_gpio_pin,0);//led3	
	set_pin_mux(plat_led_gpio->led4_group,plat_led_gpio->led4_gpio_pin,0);//led4	
#ifdef DEBUG
	printk("led probe...\n");
#endif
	return 0;
}



static int led_remove(struct platform_device *pdev)
{
	sysfs_remove_group(led_kobj, &led_group);
	kobject_del(led_kobj);
        clear_GPIO(plat_led_gpio->led1_group,plat_led_gpio->led1_gpio_pin);
        clear_GPIO(plat_led_gpio->led2_group,plat_led_gpio->led2_gpio_pin);
        clear_GPIO(plat_led_gpio->led3_group,plat_led_gpio->led3_gpio_pin);
        clear_GPIO(plat_led_gpio->led4_group,plat_led_gpio->led4_gpio_pin);
	printk("led_remove.. \n");
	return 0;
}

static struct platform_driver led_platform_driver = {
	.probe= led_probe,
	.remove= led_remove,
	.driver= {
	       .name= "led_driver_device",
	       .owner= THIS_MODULE,

	},


};




static int kobj_led_init(void)
{
	platform_driver_register(&led_platform_driver);
#ifdef DEBUG
	printk("kobj led init...\n");
#endif

	return 0;
}
 
static int  kobj_led_exit(void)
{
	platform_driver_unregister(&led_platform_driver);
#ifdef DEBUG
	printk("kobj led exit...\n");
#endif
	return 0;
}
 
module_init(kobj_led_init);
module_exit(kobj_led_exit);
