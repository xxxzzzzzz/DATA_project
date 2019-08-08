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
#include "ds2401.h"
//#define DEBUG

static struct Cdata_ds2401_GPIO_definit
{
	int ds2401_data_GPIOgroup; //pin in where group
	int ds2401_data_pin; //pin group to single pin
};

static struct Cdata_ds2401_GPIO_definit *plat_gpio_data;

static int ds2401_open(struct inode *inode, struct file *file){
#ifdef DEBUG
	printk("ds24011_open\n");
#endif	
	set_pin_mux(plat_gpio_data->ds2401_data_GPIOgroup,plat_gpio_data->ds2401_data_pin,0);
	return 0;
}

//static ssize_t ds2401_read(struct file *filp, char __user *buf,size_t count, loff_t *f_pos)
static ssize_t ds2401k_read(struct file *filp, char __user *buf,size_t count, loff_t *f_pos)
{
#ifdef DEBUG	
	unsigned char i = 0;
#endif
	unsigned char CRC;
	unsigned char data[8];
	CRC = ds2401_readID(data,plat_gpio_data->ds2401_data_GPIOgroup,plat_gpio_data->ds2401_data_pin);
#ifdef DEBUG
	for(i = 0;i<8;i++)
	{
		printk("kernel read data = %d  %d\n",i,data[i]);
	}
	printk("kernel CRC = %d\n",CRC);
#endif
	if(CRC == data[7])
	{

		if(copy_to_user(buf, data, count)!=0)
			return -EFAULT;
		else
			return 1;	
	}
	else
	{
		printk("CRC check failed\n");
		return -1;
	}
}

static unsigned int ds2401_poll(struct file *file,struct poll_table_struct *wait)
{
	unsigned long data = 0;
	unsigned int mask = 0;

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

static int ds2401_release(struct inode *inode, struct file *file)
{
	clear_GPIO(plat_gpio_data->ds2401_data_GPIOgroup,plat_gpio_data->ds2401_data_pin);
#ifdef DEBUG
	printk("ds2401_release\n");
#endif
	return 0;
}
static struct file_operations ds2401_fops = 
{
	.owner = THIS_MODULE,
	.open = ds2401_open,
	.read = ds2401k_read,
	.release = ds2401_release,
	.poll = ds2401_poll,
};

/*sensor srinal number*/
static struct cdev ds2401_cdev;

/************************/
dev_t ds2401_dev_id;
static struct class *ds2401_class;
static int major;
static int ds2401_probe(struct platform_device *dev)
{
	int ret = 0;
	plat_gpio_data = dev->dev.platform_data;

#ifdef DEBUG
	printk("enter probe\n");
	printk("ds2401_data_GPIOgroup = %d\n",plat_gpio_data->ds2401_data_GPIOgroup);
	printk("ds2401_data_pin = %d\n",plat_gpio_data->ds2401_data_pin);
#endif
	if(major)
	{
		ds2401_dev_id = MKDEV(major,0);
		ret = register_chrdev_region(ds2401_dev_id, 1, "ds2401");
	}
	else
	{
		ret = alloc_chrdev_region(&ds2401_dev_id, 0, 1, "ds2401");
		major = MAJOR(ds2401_dev_id); 	
	}

	/*sensor srinal number*/
	cdev_init(&ds2401_cdev,&ds2401_fops);
	ret = cdev_add(&ds2401_cdev, ds2401_dev_id, 1);
	if(ret<0)
		printk("cdev_add ds2401_cdev error\n");

	ds2401_class=class_create(THIS_MODULE, "ds2401");	
	device_create(ds2401_class,NULL,MKDEV(major,0),NULL,"ds2401");
	return 0;
}


static int ds2401_remove(struct platform_device *dev)
{
#ifdef DEBUG
	printk("enter remove\n");
#endif
	/*sensor srinal number*/
	device_destroy(ds2401_class, MKDEV(major,0));
	/***********************/

	class_destroy(ds2401_class);

	/*sensor srinal number*/
	cdev_del(&ds2401_cdev);
	unregister_chrdev_region(MKDEV(major,0), 1);
	/**/
	return 0;
}

static struct platform_driver ds2401_driver = {
	.probe = ds2401_probe,
	.remove = ds2401_remove,
	.driver = {
		.name = "ds2401_driver_device",
		.owner = THIS_MODULE,
	},
};

static int __init ds2401_init(void){

	platform_driver_register(&ds2401_driver);
#ifdef DEBUG
	printk("enter ds2401_driver ds2401_init\n");
#endif
	return 0;
}
static int __exit ds2401_exit(void){

	platform_driver_unregister(&ds2401_driver);
#ifdef DEBUG
	printk("enter ds2401_driver ds2401_exit\n");
#endif
	return 0;
}
module_init(ds2401_init);
module_exit(ds2401_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("xzz");
