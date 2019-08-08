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

struct hw_lowPowerCPUIO_GPIO_definit
{
	int hw_lowPowerCPUIO_group;
	int hw_lowPowerCPUIO_gpio_pin;
};

static struct hw_lowPowerCPUIO_GPIO_definit *plat_io_gpio;
/**********************************hw_lowPowerCPUIO1 show store***************************************************/
static char hw_lowPowerCPUIO1_status[3];
static ssize_t hw_lowPowerCPUIO1_show(struct kobject *kobj,struct kobj_attribute *attr, char *buf)
{
#ifdef DEBUG	
	printk("enter hw_lowPowerCPUIO1_show\n");
#endif	   
 	return sprintf(buf, "%s\n", hw_lowPowerCPUIO1_status); 
}
 
static ssize_t hw_lowPowerCPUIO1_store(struct kobject *kobj,struct kobj_attribute *attr, const char *buf, size_t count)
{
#ifdef DEBUG
	printk("hw_lowPowerCPUIO1 write: %s\n",buf);
#endif
	if(strcmp(buf,"1")==true){
		write_GPIO(plat_io_gpio->hw_lowPowerCPUIO_group,plat_io_gpio->hw_lowPowerCPUIO_gpio_pin,1);	
		strcpy(hw_lowPowerCPUIO1_status,"1");
	}
	else
	{
		write_GPIO(plat_io_gpio->hw_lowPowerCPUIO_group,plat_io_gpio->hw_lowPowerCPUIO_gpio_pin,0);	
		strcpy(hw_lowPowerCPUIO1_status,"0");
	}
	
	return count;
}

static struct kobj_attribute hw_lowPowerCPUIO1_attr = __ATTR(CPUioCtl, 0644,hw_lowPowerCPUIO1_show, hw_lowPowerCPUIO1_store);
static struct attribute *hw_lowPowerCPUIO_attribute[] = {
	&hw_lowPowerCPUIO1_attr.attr,
	NULL,

};

static struct attribute_group hw_lowPowerCPUIO_group = {
	.attrs = hw_lowPowerCPUIO_attribute,

};

static struct kobject *hw_lowPowerCPUIO_kobj = NULL;
static int __devinit hw_lowPowerCPUIO_probe(struct platform_device *pdev)
{
	int ret = 0;
	plat_io_gpio = pdev->dev.platform_data;

#ifdef DEBUG
	printk("hw_lowPowerCPUIO1 gpio_group = %d\n",plat_io_gpio->hw_lowPowerCPUIO_group);
	printk("hw_lowPowerCPUIO1 gpio_pin = %d\n",plat_io_gpio->hw_lowPowerCPUIO_gpio_pin);

#endif
	hw_lowPowerCPUIO_kobj = kobject_create_and_add("hw_lowPower", hw_lowPowerCPUIO_kobj);//在/sys/目录下创建一个led目录
	if (!hw_lowPowerCPUIO_kobj)
		return -ENOMEM;
	ret = sysfs_create_group(hw_lowPowerCPUIO_kobj, &hw_lowPowerCPUIO_group);
	if (ret)
		kobject_put(hw_lowPowerCPUIO_kobj);

	set_pin_mux(plat_io_gpio->hw_lowPowerCPUIO_group,plat_io_gpio->hw_lowPowerCPUIO_gpio_pin,0);//led1
#ifdef DEBUG
	printk("hw_lowPowerCPUIO probe...\n");
#endif
	return 0;
}



static int hw_lowPowerCPUIO_remove(struct platform_device *pdev)
{
	sysfs_remove_group(hw_lowPowerCPUIO_kobj, &hw_lowPowerCPUIO_group);
	kobject_del(hw_lowPowerCPUIO_kobj);
        clear_GPIO(plat_io_gpio->hw_lowPowerCPUIO_group,plat_io_gpio->hw_lowPowerCPUIO_gpio_pin);
	printk("hw_lowPowerCPUIO_remove.. \n");
	return 0;
}

static struct platform_driver hw_lowPowerCPUIO_platform_driver = {
	.probe= hw_lowPowerCPUIO_probe,
	.remove= hw_lowPowerCPUIO_remove,
	.driver= {
	       .name= "hw_lowPowerCPUIO_driver_device",
	       .owner= THIS_MODULE,

	},


};




static int kobj_hw_lowPowerCPUIO_init(void)
{
	platform_driver_register(&hw_lowPowerCPUIO_platform_driver);
#ifdef DEBUG
	printk("kobj hw_lowPowerCPUIO init...\n");
#endif

	return 0;
}
 
static void  kobj_hw_lowPowerCPUIO_exit(void)
{
	platform_driver_unregister(&hw_lowPowerCPUIO_platform_driver);
#ifdef DEBUG
	printk("kobj hw_lowPowerCPUIO exit...\n");
#endif
}
 
module_init(kobj_hw_lowPowerCPUIO_init);
module_exit(kobj_hw_lowPowerCPUIO_exit);
