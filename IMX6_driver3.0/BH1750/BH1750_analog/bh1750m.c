

/*author: xiangzizhou ,versions: wbed1.0 */
/*bh1750 sensor 4 individual*/

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
#include "bh1750_rw.h"
/*包含函数device_create 结构体class等头文件*/
#include <linux/device.h>
#include <linux/gpio.h>
#include <linux/delay.h>
#include <linux/io.h>
#include <linux/of_gpio.h>
MODULE_LICENSE("Dual BSD/GPL");
/*声明是开源的，没有内核版本限制*/


#define IMX_GPIO_NR(bank, nr)		(((bank) - 1) * 32 + (nr))
#define GPIO1_3 IMX_GPIO_NR(1, 3) 
#define GPIO1_6 IMX_GPIO_NR(1, 6) 

#define clk GPIO1_3
#define sda GPIO1_6


static int major;
static int bh1750_open (struct inode *inode, struct file *file){
static int ret;
unsigned int temp;
        ret = gpio_request_one(sda,GPIOF_OUT_INIT_HIGH,"fec-phy-reset");
	if(ret<0){printk("sda request failed!\n");}
	ret = gpio_request_one(clk,GPIOF_OUT_INIT_HIGH,"fec-phy-reset");
	if(ret<0){printk("clk request failed!\n");}
        printk("bh1750 open\n");
        return 0; //this return important if not appear printf luanma	
}

/*bh1750 read to user*/
static ssize_t bh1750_read(struct file *filp, char __user *buf,size_t count, loff_t *f_pos){
static  unsigned int data;
    
        
    data  = bh1750();
   if(copy_to_user(buf, &data, count)!=0){
      return -EFAULT;
         } 
      return 1;
}




int bh1750_release(struct inode *inode, struct file *file){
      gpio_free(GPIO1_3);
      gpio_free(GPIO1_6);
      printk("close bh17501\n");
      return 0;

}









static struct file_operations bh1750_fops = {

.owner = THIS_MODULE,
.open = bh1750_open,
.read = bh1750_read,
.release = bh1750_release,

};


static struct cdev bh1750_dev;
static struct class *bh1750_class;

static int bh1750_init(void){
int result;
dev_t dev;

printk("enter bh1750 \n");
if(major){
     dev=MKDEV(major,0);
     result = register_chrdev_region(dev, 1, "bh1750");
     }

else {
     result = alloc_chrdev_region(&dev, 0, 1, "bh1750");
     major = MAJOR(dev);
						
     } 



/*bh1750 add*/
    cdev_init(&bh1750_dev, &bh1750_fops);
    result = cdev_add(&bh1750_dev, dev, 1);



    bh1750_class=class_create(THIS_MODULE, "bh1750");

    device_create(bh1750_class,NULL,MKDEV(major,0),NULL,"bh1750");

    return 0;
}





static int bh1750_exit(void){

    printk("exit bh1750 \n");

    device_destroy(bh1750_class, MKDEV(major,0));
    
    class_destroy(bh1750_class);
    
    cdev_del(&bh1750_dev);
    unregister_chrdev_region(MKDEV(major,0), 1);
    

    return 0;
}


module_init(bh1750_init);
module_exit(bh1750_exit);
