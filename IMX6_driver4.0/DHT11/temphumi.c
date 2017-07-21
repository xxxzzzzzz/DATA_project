
/*author: xiangzizhou ,versions: wbed1.0 */
/*temphumi sensor 4 individual*/

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
#include "temphumi.h"
/*包含函数device_create 结构体class等头文件*/
#include <linux/device.h>
#include <linux/gpio.h>
#include <linux/delay.h>
#include <linux/io.h>
#include <linux/of_gpio.h>
MODULE_LICENSE("Dual BSD/GPL");
/*声明是开源的，没有内核版本限制*/


#define IMX_GPIO_NR(bank, nr)		(((bank) - 1) * 32 + (nr))
#define GPIO1_2 IMX_GPIO_NR(1, 2) 
#define DHT11_DQ GPIO1_2


static atomic_t dht11_atomic = ATOMIC_INIT(1); 
static spinlock_t dht11_lock;
static int major;
static int temphumi_open (struct inode *inode, struct file *file){
  int ret;
 

     if(!atomic_dec_and_test(&dht11_atomic)){
       atomic_inc(&dht11_atomic);
       return -EBUSY;

      }

     spin_lock_init(&dht11_lock); 
     ret= gpio_request_one(DHT11_DQ,GPIOF_OUT_INIT_LOW,"fec-phy-reset");
     if(ret<0){printk("DHT11 gpio request failed!!\n");}
     printk("temphumi open\n");
     while(DHT11_Init()){          //复位DHT11
     printk("dht11 init failed!!\n"); 
      } 

      return 0; //this return important if not appear printf luanma	
}

/*temphumi read to user*/
static ssize_t temphumi_read(struct file *filp, char __user *buf,size_t count, loff_t *f_pos){
static  unsigned char data[2];
static  unsigned char temperature,humidity;


   spin_lock(&dht11_lock);
   DHT11_Read_Data(&temperature,&humidity);
   data[0]=temperature;
   data[1]=humidity;
   spin_unlock(&dht11_lock);
   if(copy_to_user(buf, data, count)!=0){
      return -EFAULT;
      } 
      return 1;
}




int temphumi1_release(struct inode *inode, struct file *file){
      gpio_free(GPIO1_2);
      atomic_inc(&dht11_atomic);
      printk("close temphumi1\n");
      return 0;

}









static struct file_operations temphumi_fops = {

.owner = THIS_MODULE,
.open = temphumi_open,
.read = temphumi_read,
.release = temphumi1_release,

};


static struct cdev temphumi_dev;
static struct class *temphumi_class;

static int temphumi_init(void){
int result;
dev_t dev;

printk("enter temphumi \n");
if(major){
     dev=MKDEV(major,0);
     result = register_chrdev_region(dev, 1, "temphumi");
     }

else {
     result = alloc_chrdev_region(&dev, 0, 1, "temphumi");
     major = MAJOR(dev);
						
     } 



/*temphumi add*/
    cdev_init(&temphumi_dev, &temphumi_fops);
    result = cdev_add(&temphumi_dev, dev, 1);



    temphumi_class=class_create(THIS_MODULE, "temphumi");

    device_create(temphumi_class,NULL,MKDEV(major,0),NULL,"threesboy_temphumi");

    return 0;
}





static int temphumi_exit(void){

    printk("exit temphumi \n");

    device_destroy(temphumi_class, MKDEV(major,0));
    
    class_destroy(temphumi_class);
    
    cdev_del(&temphumi_dev);
    unregister_chrdev_region(MKDEV(major,0), 1);
    

    return 0;
}


module_init(temphumi_init);
module_exit(temphumi_exit);
