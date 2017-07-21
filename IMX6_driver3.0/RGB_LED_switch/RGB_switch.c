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
#include <linux/of_gpio.h>

MODULE_AUTHOR("xiang");
MODULE_LICENSE("Dual BSD/GPL");

#define IMX_GPIO_NR(bank, nr)           (((bank) - 1) * 32 + (nr))
#define RGB_led_swicth IMX_GPIO_NR(1, 7) //RGB switch
static char status[3];

static ssize_t led_show(struct kobject *kobj,struct kobj_attribute *attr, char *buf)
{
	
	printk("enter led_show\n");
	   
 	return sprintf(buf, "%s\n", status);
}
 
static ssize_t led_store(struct kobject *kobj,struct kobj_attribute *attr, const char *buf, size_t count)
{
	printk("kobj_test_store\n");
	printk("write: %s\n",buf);
	if(strcmp(buf,"1")==true){
	
	gpio_set_value(RGB_led_swicth,1);
	strcpy(status,"1");
	}
	else
	{
	
	gpio_set_value(RGB_led_swicth,0);
	strcpy(status,"0");
	}
	
	return count;
}

static struct kobj_attribute RGB_LED_SW = __ATTR(RGB_led, 0644,led_show, led_store);//在目录里面创建的文件名RGB_led就是一个文件名
 
static struct attribute *RGB_SW[] = {
	&RGB_LED_SW.attr,
	NULL,

};

static struct attribute_group RGB_SW_group = {
	.attrs = RGB_SW,

};

static struct kobject *RGB_sw_kobj;
static int kobj_test_init(void)
{
	int ret;
	printk("RGB_SW init.\n");
	//...............RGB switch...........init
	ret = gpio_request_one(RGB_led_swicth,GPIOF_OUT_INIT_HIGH, "fec-phy-reset");
	if(ret){printk("gpio RGB_led_swicth request failed\n");}
	   else {printk("gpio  RGB_led_swicth  request success\n");}

	RGB_sw_kobj = kobject_create_and_add("RGB_LED_switch", RGB_sw_kobj);//在/sys/目录下创建一个目录 RGB_LED_switch就是目录名称
	if (!RGB_sw_kobj)
		return -ENOMEM;
	ret = sysfs_create_group(RGB_sw_kobj, &RGB_SW_group);
	if (ret)
		kobject_put(RGB_sw_kobj);
	return 0;

}
 
static int  kobj_test_exit(void)
{
	printk("RGB_SW exit.\n");
	gpio_free(RGB_led_swicth);
	sysfs_remove_group(RGB_sw_kobj, &RGB_SW_group);
	kobject_del(RGB_sw_kobj);
	return 0;

}
 
module_init(kobj_test_init);
module_exit(kobj_test_exit);
