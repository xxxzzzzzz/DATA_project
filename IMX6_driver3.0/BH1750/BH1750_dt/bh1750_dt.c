#include<linux/init.h> 
/*包含初始化加载模块的头文件,代码中的MODULE_LICENSE在此头文件中*/
#include <linux/module.h>
/*定义module_param module_param_array的头文件*/
//#include <linux/moduleparam.h>
///*定义module_param module_param_array中perm的头文件*/
////#include <linux/stat.h>
///*三个字符设备函数*/
#include <linux/fs.h>
///*MKDEV转换设备号数据类型的宏定义*/
#include <linux/kdev_t.h>
///*定义字符设备的结构体*/
#include <linux/cdev.h>
///*分配内存空间函数头文件*/
////#include <linux/slab.h>
#include <linux/kernel.h>
#include <linux/uaccess.h>
#include <linux/mutex.h>
///*包含函数device_create 结构体class等头文件*/
#include <linux/device.h>
#include <linux/gpio.h>
#include <linux/delay.h>
#include <linux/io.h>
#include <linux/of_gpio.h>
#include <linux/of_platform.h>
#include <linux/i2c.h>
MODULE_LICENSE("Dual BSD/GPL");




static int major=0;
struct i2c_client *bh1750_client;

static atomic_t bh1750_atomic = ATOMIC_INIT(1);
static DEFINE_MUTEX(bh1750_mutex);  
static int bh1750_open(struct inode *inode, struct file *file){

unsigned char regaddr=0x01;
unsigned char regdata=0x10;
     if(!atomic_dec_and_test(&bh1750_atomic)){
       atomic_inc(&bh1750_atomic);
       return -EBUSY;

      }
printk("enter bh1750_open\n");

i2c_master_send(bh1750_client, &regaddr,1);
i2c_master_send(bh1750_client, &regdata,1);


return 0;
}

static ssize_t bh1750_read(struct file *filp, char __user *buf,size_t count, loff_t *f_pos){

	unsigned char readbuf[3];
	unsigned int temp;
	mutex_lock(&bh1750_mutex);
	i2c_master_recv(bh1750_client, readbuf, 2);
	temp=readbuf[0]; 
	temp=(temp<<8)+readbuf[1];

	mutex_unlock(&bh1750_mutex);
	if(copy_to_user(buf, &temp, sizeof(&temp))!=0){
	
	   return -EFAULT;
	}

	return 0;
}

int bh1750_release(struct inode *inode, struct file *file){
atomic_inc(&bh1750_atomic);
printk("enter bh1750_close\n");
return 0;
}


static struct file_operations bh1750_fops = {

   .owner = THIS_MODULE,
   .open = bh1750_open,
   .read = bh1750_read,
   .release = bh1750_release,


};


static struct cdev  bh1750_dev;
static struct class *bh1750_class;
static int bh1750_probe(struct i2c_client *client,const struct i2c_device_id *id){

  int result;
  dev_t dev;
  printk("bh1750dt probe\n");

  bh1750_client = client;
	  


  if(major){
   dev=MKDEV(major,0);
   result = register_chrdev_region(dev, 1, "bh1750");
		       
           }

   else {
   result = alloc_chrdev_region(&dev, 0, 1, "bh1750");
   major = MAJOR(dev);

        }
             
  cdev_init(&bh1750_dev,&bh1750_fops);
  result = cdev_add(&bh1750_dev,dev,1);
  bh1750_class=class_create(THIS_MODULE, "bh1750");
  device_create(bh1750_class,NULL,MKDEV(major,0),NULL,"bh1750");
	return 0;
}

static int bh1750_remove(struct platform_device *pdev){
	
   printk("bh1750dt remove\n");
   device_destroy(bh1750_class, MKDEV(major,0));
   class_destroy(bh1750_class);
   cdev_del(&bh1750_dev);
   unregister_chrdev_region(MKDEV(major,0), 1);
	return 0;

}


static const struct i2c_device_id bh1750_id[] = {
	{ "bh1750", 0  },
	{  }

};

static struct of_device_id bh1750dt[]={
	{ .compatible = "bh1750",},
	{},

	};

MODULE_DEVICE_TABLE(of, bh1750dt);


static struct i2c_driver bh1750_driver = {
	.driver = {
	.name= "bh1750",
	.owner= THIS_MODULE,
	.of_match_table= bh1750dt,
											
	},
	.id_table= bh1750_id,
	.probe= bh1750_probe,
	.remove = bh1750_remove,
};



static int bh1750_init(void){
  printk("enter bh1750\n");
  i2c_add_driver(&bh1750_driver);
  return 0;
}

static int bh1750_exit(void){

   printk("enter bh1750\n");
   i2c_del_driver(&bh1750_driver);
   return 0;

}



	



module_init(bh1750_init);
module_exit(bh1750_exit);


