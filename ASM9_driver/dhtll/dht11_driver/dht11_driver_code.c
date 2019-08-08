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
#include "dht11.h"
//#define DEBUG

static struct Cdata_dht11_GPIO_definit
{
	int dht11_data_GPIOgroup; //pin in where group
	int dht11_data_pin; //pin group to single pin
};

static struct Cdata_dht11_GPIO_definit *plat_gpio_data;

static int dht11_open(struct inode *inode, struct file *file){
#ifdef DEBUG
	printk("dht111_open\n");
#endif	
	set_pin_mux(plat_gpio_data->dht11_data_GPIOgroup,plat_gpio_data->dht11_data_pin,0);
	while(DHT11_Init(plat_gpio_data->dht11_data_GPIOgroup,plat_gpio_data->dht11_data_pin)) //复位DHT11
	{
      		printk("dht11 init failed!!\n");		
       	}
	return 0;
}

static ssize_t dht11_read(struct file *filp, char __user *buf,size_t count, loff_t *f_pos)
{
	unsigned char data[2];
	unsigned char temperature,humidity;
	int ret = 0;
     	ret = DHT11_Read_Data(&temperature,&humidity,plat_gpio_data->dht11_data_GPIOgroup,plat_gpio_data->dht11_data_pin);
	if(ret<0)
		printk("DHT11 Read Data failed\n");
	data[0]=temperature;
	data[1]=humidity;
#ifdef DEBUG
	printk("kernel read temp = %d humi = %d\n",data[0],data[1]);	
#endif
	if(copy_to_user(buf, data, count)!=0)
		return -EFAULT;
	else
		return 1;
}

static unsigned int dht11_poll(struct file *file,struct poll_table_struct *wait)
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

static int dht11_release(struct inode *inode, struct file *file)
{
	clear_GPIO(plat_gpio_data->dht11_data_GPIOgroup,plat_gpio_data->dht11_data_pin);
#ifdef DEBUG
	printk("dht11_release\n");
#endif
	return 0;
}
static struct file_operations dht11_fops = 
{
	.owner = THIS_MODULE,
	.open = dht11_open,
	.read = dht11_read,
	.release = dht11_release,
	.poll = dht11_poll,
};

/*sensor srinal number*/
static struct cdev dht11_cdev;

/************************/
dev_t dht11_dev_id;
static struct class *dht11_class;
static int major;
static int dht11_probe(struct platform_device *dev)
{
	int ret = 0;
	plat_gpio_data = dev->dev.platform_data;

#ifdef DEBUG
	printk("enter probe\n");
	printk("dht11_data_GPIOgroup = %d\n",plat_gpio_data->dht11_data_GPIOgroup);
	printk("dht11_data_pin = %d\n",plat_gpio_data->dht11_data_pin);
#endif
	if(major)
	{
		dht11_dev_id = MKDEV(major,0);
		ret = register_chrdev_region(dht11_dev_id, 1, "dht11");
	}
	else
	{
		ret = alloc_chrdev_region(&dht11_dev_id, 0, 1, "dht11");
		major = MAJOR(dht11_dev_id); 	
	}

	/*sensor srinal number*/
	cdev_init(&dht11_cdev,&dht11_fops);
	ret = cdev_add(&dht11_cdev, dht11_dev_id, 1);
	if(ret<0)
		printk("cdev_add dht11_cdev error\n");

	dht11_class=class_create(THIS_MODULE, "dht11");	
	device_create(dht11_class,NULL,MKDEV(major,0),NULL,"dht11");
	return 0;
}


static int dht11_remove(struct platform_device *dev)
{
#ifdef DEBUG
	printk("enter remove\n");
#endif
	/*sensor srinal number*/
	device_destroy(dht11_class, MKDEV(major,0));
	/***********************/

	class_destroy(dht11_class);

	/*sensor srinal number*/
	cdev_del(&dht11_cdev);
	unregister_chrdev_region(MKDEV(major,0), 1);
	/**/
	return 0;
}

static struct platform_driver dht11_driver = {
	.probe = dht11_probe,
	.remove = dht11_remove,
	.driver = {
		.name = "dht11_driver_device",
		.owner = THIS_MODULE,
	},
};

static int __init dht11_init(void){

	platform_driver_register(&dht11_driver);
#ifdef DEBUG
	printk("enter dht11_driver dht11_init\n");
#endif
	return 0;
}
static int __exit dht11_exit(void){

	platform_driver_unregister(&dht11_driver);
#ifdef DEBUG
	printk("enter dht11_driver dht11_exit\n");
#endif
	return 0;
}
module_init(dht11_init);
module_exit(dht11_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("xzz");
