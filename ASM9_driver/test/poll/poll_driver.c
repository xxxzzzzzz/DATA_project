
/*author: xiangzizhou ,poll*/

/*包含初始化宏定义的头文件,代码中的module_init和module_exit在此文件中*/
#include <linux/init.h>
/*包含初始化加载模块的头文件,代码中的MODULE_LICENSE在此头文件中*/
#include <linux/module.h>
/*定义module_param module_param_array的头文件*/
//#include <linux/moduleparam.h>
/*定义module_param module_param_array中perm的头文件*/
//#include <linux/stat.h>
/*三个字符设备函数*/
#include <linux/fs.h>
/*MKDEV转换设备号数据类型的宏定义*/
#include <linux/kdev_t.h>
/*定义字符设备的结构体*/
#include <linux/cdev.h>
/*分配内存空间函数头文件*/
//#include <linux/slab.h>
#include <linux/kernel.h>
#include <linux/uaccess.h>
#include <asm/uaccess.h>
/*包含函数device_create 结构体class等头文件*/
#include <linux/device.h>
#include <linux/gpio.h>
#include <linux/delay.h>
#include <linux/io.h>
#include <linux/wait.h>　
#include <linux/poll.h>

MODULE_LICENSE("Dual BSD/GPL");
/*声明是开源的，没有内核版本限制*/



static DECLARE_WAIT_QUEUE_HEAD(poll_waitq);
static DECLARE_WAIT_QUEUE_HEAD(poll_waitq2);
/*poll1 open*/
static int poll_open(struct inode *inode, struct file *file)
{     
    return 0; //this return important if not appear printf luanma	
}

/*poll1 read to user*/
static ssize_t poll_read(struct file *filp, char __user *buf,size_t count, loff_t *f_pos){
	int key_value = 10;
	if(copy_to_user(buf,&key_value,count)!=0){

		return -EFAULT;
	}
        return 0;
}

	

int poll1_release(struct inode *inode, struct file *file){
        printk("close poll1\n");
        return 0;

}

static unsigned  poll_poll(struct file *filp, struct poll_table *wait){
        unsigned int mask=0;
	mask |= POLLIN|POLLRDNORM;//这里给int变量返回一个POLLIN，如果应用层是poll机制的POLLIN就不会阻塞，如果应用层用的select机制，也不会阻塞
//	mask |= POLLOUT|POLLRDNORM;
        return mask;
}

static int poll_open2(struct inode *inode, struct file *file)
{     
   	return 0; //this return important if not appear printf luanma	
}

int poll1_release2(struct inode *inode, struct file *file){
        printk("close poll2\n");
        return 0;

}
static unsigned  poll_poll2(struct file *filp, struct poll_table *wait){
        unsigned int mask=0;

	mask |= POLLIN|POLLRDNORM;//这里给int变量返回一个POLLIN，如果应用层是POLLIN，就不会阻塞
//	mask |= POLLOUT|POLLRDNORM;
        return mask;
}
static struct file_operations poll_fops = {

	.owner = THIS_MODULE,
	.open = poll_open,
	.read = poll_read,
	.release = poll1_release,
	.poll = poll_poll,//主要是要在字符设备里面加poll函数
};

static struct file_operations poll_fops2 = {

	.owner = THIS_MODULE,
	.open = poll_open2,
	.release = poll1_release2,
	.poll = poll_poll2,//主要是要在字符设备里面加poll函数
};


static int major;
static struct cdev poll_dev;
static struct cdev poll_dev2;
static struct class *poll_class;
static int __init poll_init(void){
	int result;
	dev_t dev;

	printk("enter poll \n");
	if(major){
	dev=MKDEV(major,0);
	result = register_chrdev_region(dev, 1, "poll");
	}

	else {
	result = alloc_chrdev_region(&dev, 0, 1, "poll");
	major = MAJOR(dev);
	} 
	/*poll1 add*/
	cdev_init(&poll_dev, &poll_fops);
	result = cdev_add(&poll_dev, dev, 1);

    	dev=MKDEV(major,2);//获取次设备号,次设备号从2开始
    	register_chrdev_region(dev, 1, "poll2");//分配一个次设备号，所以就是2
    	cdev_init(&poll_dev2, &poll_fops2);
    	result = cdev_add(&poll_dev2, dev, 1);

	poll_class=class_create(THIS_MODULE, "poll_class");
	device_create(poll_class,NULL,MKDEV(major,0),NULL,"poll_dev");//驱动在/dev下创建设备节点
	device_create(poll_class,NULL,MKDEV(major,2),NULL,"poll_dev2");//驱动在/dev下创建设备节点
    	return 0;
}


static int __exit poll_exit(void){

	printk("exit poll \n");
	device_destroy(poll_class, MKDEV(major,0));    
	device_destroy(poll_class, MKDEV(major,2));    
	class_destroy(poll_class);
	cdev_del(&poll_dev);
	cdev_del(&poll_dev2);
	unregister_chrdev_region(MKDEV(major,0), 1);
	unregister_chrdev_region(MKDEV(major,2), 1);
	return 0;
}


module_init(poll_init);
module_exit(poll_exit);
