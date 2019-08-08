#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/kernel.h>
#include <linux/uaccess.h>
#include <linux/mutex.h>
#include <linux/device.h>
#include <linux/spinlock.h>
#include <linux/gpio.h>
#include <linux/delay.h>
#include <linux/io.h>
#include <linux/platform_device.h>
#include<mach/pincontrol.h>
#include <linux/wait.h>
#include <linux/poll.h>
#include "hx711_iic.h"

//#define DEBUG

static struct Cdata_1hm_GPIO_definit
{
	int weight1_CLK_GPIOgroup; //pin in where group
	int weight1_CLK_pin; //pin group to single pin
	int weight1_data_GPIOgroup; //pin in where group
	int weight1_data_pin; //pin group to single pin
};

static struct Cdata_1hm_GPIO_definit *plat_gpio_data;

static int weight1_open (struct inode *inode, struct file *file){
#ifdef DEBUG
	printk("weight1_open\n");
#endif	
	set_pin_mux(plat_gpio_data->weight1_CLK_GPIOgroup,plat_gpio_data->weight1_CLK_pin,0);
	set_pin_mux(plat_gpio_data->weight1_data_GPIOgroup,plat_gpio_data->weight1_data_pin,0);

	return 0;
}

static ssize_t weight1_read(struct file *filp, char __user *buf,size_t count, loff_t *f_pos)
{
	static unsigned long data;
	if(count!=sizeof(&data))
	{
		printk("copy to user faled!!\n");
		return -EINVAL;
	}
      
	data = get_weight(plat_gpio_data->weight1_CLK_GPIOgroup,plat_gpio_data->weight1_CLK_pin,
			plat_gpio_data->weight1_data_GPIOgroup,plat_gpio_data->weight1_data_pin);
#ifdef DEBUG
	printk("kernel read = %x\n",data);	
#endif
	if(copy_to_user(buf, &data, sizeof(&data))!=0)
		return -EFAULT;
	else
		return 1;
}

static unsigned int weight1_poll(struct file *file,struct poll_table_struct *wait)
{
	unsigned long data = 0;
	unsigned int mask = 0;

	data = get_weight(plat_gpio_data->weight1_CLK_GPIOgroup,plat_gpio_data->weight1_CLK_pin,
			plat_gpio_data->weight1_data_GPIOgroup,plat_gpio_data->weight1_data_pin);
#ifdef DEBUG
	printk("poll read = %x\n",data);	
#endif
	if(data == 0xfffffff)
	{
		mask = 0;
		return mask;
	}
	else
	{
		mask |= POLLIN|POLLRDNORM;		
		return mask;
	}
	
}

static int weight1_release(struct inode *inode, struct file *file)
{
	clear_GPIO(plat_gpio_data->weight1_CLK_GPIOgroup,plat_gpio_data->weight1_CLK_pin);
	clear_GPIO(plat_gpio_data->weight1_data_GPIOgroup,plat_gpio_data->weight1_data_pin);
	return 0;
}
static struct file_operations weight1_fops = 
{
	.owner = THIS_MODULE,
	.open = weight1_open,
	.read = weight1_read,
	.release = weight1_release,
	.poll = weight1_poll,
};

/*sensor srinal number*/
static struct cdev weight1_cdev;

/************************/
dev_t weight_dev_id;
static struct class *weight_class;
static int major;
static int weight_probe(struct platform_device *dev)
{
	int ret = 0;
	plat_gpio_data = dev->dev.platform_data;

#ifdef DEBUG
	printk("enter probe\n");
	printk("weight1_CLK_GPIOgroup = %d\n",plat_gpio_data->weight1_CLK_GPIOgroup);
	printk("weight1_CLK_pin = %d\n",plat_gpio_data->weight1_CLK_pin);
	printk("weight1_data_GPIOgroup = %d\n",plat_gpio_data->weight1_data_GPIOgroup);
	printk("weight1_data_pin = %d\n",plat_gpio_data->weight1_data_pin);
#endif
	if(major)
	{
		weight_dev_id = MKDEV(major,0);
		ret = register_chrdev_region(weight_dev_id, 1, "weight");
	}
	else
	{
		ret = alloc_chrdev_region(&weight_dev_id, 0, 1, "weight");
		major = MAJOR(weight_dev_id); 	
	}

	/*sensor srinal number*/
	cdev_init(&weight1_cdev,&weight1_fops);
	ret = cdev_add(&weight1_cdev, weight_dev_id, 1);
	if(ret<0)
		printk("cdev_add weight1_cdev error\n");

	weight_class=class_create(THIS_MODULE, "weight");	
	device_create(weight_class,NULL,MKDEV(major,0),NULL,"threesboy_weight1");
	return 0;
}


static int weight_remove(struct platform_device *dev)
{
#ifdef DEBUG
	printk("enter remove\n");
#endif
	/*sensor srinal number*/
	device_destroy(weight_class, MKDEV(major,0));
	/***********************/

	class_destroy(weight_class);

	/*sensor srinal number*/
	cdev_del(&weight1_cdev);
	unregister_chrdev_region(MKDEV(major,0), 1);
	/**/
	return 0;
}

static struct platform_driver weight_driver = {
	.probe = weight_probe,
	.remove = weight_remove,
	.driver = {
		.name = "1hm_driver_device",
		.owner = THIS_MODULE,
	},
};

static int __init weight_init(void){

	platform_driver_register(&weight_driver);
#ifdef DEBUG
	printk("enter 1hm_driver weight_init\n");
#endif
	return 0;
}
static int __exit weight_exit(void){

	platform_driver_unregister(&weight_driver);
#ifdef DEBUG
	printk("enter 1hm_driver weight_exit\n");
#endif
	return 0;
}
module_init(weight_init);
module_exit(weight_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("xzz");
