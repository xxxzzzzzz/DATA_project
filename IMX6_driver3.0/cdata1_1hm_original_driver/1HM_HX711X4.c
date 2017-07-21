
/*author:向仔州  ,versions: wbed1.0 */
/*1hm_sensor 4 driver*/
/*cird*/

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
#include <linux/mutex.h>
/*包含函数device_create 结构体class等头文件*/
#include <linux/device.h>
#include "iic_analog.h"
#include "iic_analog2.h"
#include "iic_analog3.h"
#include "iic_analog4.h"
#include <linux/spinlock.h>
#include <linux/gpio.h>
#include <linux/delay.h>
#include <linux/io.h>
#include <linux/of_gpio.h>
MODULE_LICENSE("Dual BSD/GPL");
/*声明是开源的，没有内核版本限制*/


#define IMX_GPIO_NR(bank, nr)           (((bank) - 1) * 32 + (nr))
#define GPIO4_7 IMX_GPIO_NR(4, 7)
#define GPIO4_6 IMX_GPIO_NR(4, 6)


#define GPIO4_9 IMX_GPIO_NR(4, 9)
#define GPIO4_8 IMX_GPIO_NR(4, 8) 

#define GPIO4_11 IMX_GPIO_NR(4, 11)
#define GPIO1_8 IMX_GPIO_NR(1, 8) 

#define GPIO4_15 IMX_GPIO_NR(4, 15) 
#define GPIO4_14 IMX_GPIO_NR(4, 14) 

static atomic_t weightfd = ATOMIC_INIT(1);
static atomic_t weightfd2 = ATOMIC_INIT(1);
static atomic_t weightfd3 = ATOMIC_INIT(1);
static atomic_t weightfd4 = ATOMIC_INIT(1);
static int major;

static DEFINE_MUTEX(weight_mutex);
/*weight1 open*/
static int weight_open (struct inode *inode, struct file *file){
  int ret;
      if(!atomic_dec_and_test(&weightfd)){
       atomic_inc(&weightfd);
       return -EBUSY;

      }

      printk("weight open\n");

      ret = gpio_request_one(GPIO4_6,GPIOF_DIR_IN, "fec-phy-reset");

      if(ret){printk("hx1 gpio GPIO4_6 request failed\n");}  
      else {printk("hx1 gpio GPIO4_6  request success\n");}

      ret = gpio_request_one(GPIO4_7,GPIOF_OUT_INIT_LOW, "fec-phy-reset");
      if(ret){printk("hx1 gpio GPIO4_7 request failed\n");}
      else {printk("hx1 gpio GPIO4_7  request success\n");} 


      return 0; //this return important if not appear printf luanma	
}

/*weight1 read to user*/
static ssize_t weight_read(struct file *filp, char __user *buf,size_t count, loff_t *f_pos){
static unsigned int data;
      if(count!=sizeof(&data)){
       printk("copy to user faled!!\n");
       return -EINVAL;

      }
     mutex_lock(&weight_mutex);
     
      data= get_weight();
//    printk("weight1 original %d",data);
//  data=data*10;  //1HM not *10 or s6400 unlike
     mutex_unlock(&weight_mutex);
      if(copy_to_user(buf, &data, sizeof(&data))!=0){

            return -EFAULT;

       } 
      return 1;
}


/*weight2 open*/

static int weight2_open (struct inode *inode, struct file *file){
  int ret;

    if(!atomic_dec_and_test(&weightfd2)){
      atomic_inc(&weightfd2);
      return -EBUSY;

     }
     printk("weight2 open\n");
   

     ret = gpio_request_one(GPIO4_8,GPIOF_DIR_IN, "fec-phy-reset");
     if(ret){printk("hx2 gpio GPIO4_8 request failed\n");}  
     else {printk("hx2 gpio GPIO4_8  request success\n");}

     ret = gpio_request_one(GPIO4_9,GPIOF_OUT_INIT_LOW, "fec-phy-reset");
     if(ret){printk("hx2 gpio GPIO4_9 request failed\n");}
     else {printk("hx2 gpio GPIO4_9  request success\n");}
     

     return 0;	
}




	

/*weight2 read to user*/
static ssize_t weight2_read(struct file *filp, char __user *buf,size_t count, loff_t *f_pos){
static unsigned int data;
     if(count!=sizeof(&data)){
      printk("copy to user faled!!\n");
       return -EINVAL;

      }
     mutex_lock(&weight_mutex);
    
     data= get_weight_2();
//   printk("weight2 original %d",data);
// data=data*10;    //1HM not *10 or s6400 unlike      
     mutex_unlock(&weight_mutex);

     if(copy_to_user(buf, &data, sizeof(&data))!=0){
					
	return -EFAULT;
						               
	}

     return 1;

}
/*weight3 open*/
static int weight3_open (struct inode *inode, struct file *file){
  int ret;

     if(!atomic_dec_and_test(&weightfd3)){
       atomic_inc(&weightfd3);
       return -EBUSY;

     }
     printk("weight3 open\n");
    

     ret = gpio_request_one(GPIO1_8,GPIOF_DIR_IN, "fec-phy-reset");
     if(ret){printk("hx3 gpio GPIO1_5 request failed\n");}  
     else {printk("hx3 gpio GPIO1_5  request success\n");}

     ret = gpio_request_one(GPIO4_11,GPIOF_OUT_INIT_LOW, "fec-phy-reset");
     if(ret){printk("hx3 gpio GPIO1_4 request failed\n");}
     else {printk("hx3 gpio GPIO1_4  request success\n");}


     return 0;	
}



/*weight3 read to user*/
static ssize_t weight3_read(struct file *filp, char __user *buf,size_t count, loff_t *f_pos){
static unsigned int data;
     if(count!=sizeof(&data)){
      printk("copy to user faled!!\n");
       return -EINVAL;

      }
  mutex_lock(&weight_mutex);
     data= get_weight_3();
// data=data*10;   //1HM not *10 or s6400 unlike
  mutex_unlock(&weight_mutex);

     if(copy_to_user(buf, &data, sizeof(&data))!=0){
					
	return -EFAULT;
						               
	}


     return 1;
}
/*weight4 open*/
static int weight4_open (struct inode *inode, struct file *file){
  int ret;

    if(!atomic_dec_and_test(&weightfd4)){
      atomic_inc(&weightfd4);
      return -EBUSY;

     }
     printk("weight4 open\n");
  
     ret = gpio_request_one(GPIO4_14,GPIOF_DIR_IN, "fec-phy-reset");
     if(ret){printk("hx4 gpio GPIO4_14 request failed\n");}
     else {printk("hx4 gpio GPIO4_14  request success\n");}

     ret = gpio_request_one(GPIO4_15,GPIOF_OUT_INIT_LOW, "fec-phy-reset");
     if(ret){printk("hx4 gpio GPIO4_15 request failed\n");}
     else {printk("hx4 gpio GPIO4_15  request success\n");}

	
     return 0;

}

/*weight4 read to user*/
static ssize_t weight4_read(struct file *filp, char __user *buf,size_t count, loff_t *f_pos){
static unsigned int data;
    if(count!=sizeof(&data)){
      printk("copy to user faled!!\n");
       return -EINVAL;

     }
    mutex_lock(&weight_mutex);
    data= get_weight_4();
 //   data=data*10;    //1HM not *10 or s6400 unlike
    mutex_unlock(&weight_mutex);

    if(copy_to_user(buf, &data, sizeof(&data))!=0){
	 					
	  return -EFAULT;

							        
	}


    return 1;

}


int weight1_release(struct inode *inode, struct file *file){
	gpio_free(GPIO4_7);
	gpio_free(GPIO4_6);
        atomic_inc(&weightfd);
        printk("close weight1\n");
        return 0;

}




int weight2_release(struct inode *inode, struct file *file){
	
	gpio_free(GPIO4_8);
        gpio_free(GPIO4_9);
        atomic_inc(&weightfd2);
	printk("close weight2\n");
        return 0;

}

int weight3_release(struct inode *inode, struct file *file){
	
	gpio_free(GPIO4_11);
	gpio_free(GPIO1_8);
        atomic_inc(&weightfd3);
	printk("close weight3\n");
        return 0;

}

int weight4_release(struct inode *inode, struct file *file){
	
	gpio_free(GPIO4_14);
	gpio_free(GPIO4_15);
        atomic_inc(&weightfd4);
	printk("close weight4\n");
        return 0;

}





static struct file_operations weight_fops = {

.owner = THIS_MODULE,
.open = weight_open,
.read = weight_read,
.release = weight1_release,

};
static struct file_operations weight2_fops = {

.owner = THIS_MODULE,
.open = weight2_open,
.read = weight2_read,
.release = weight2_release,
};

static struct file_operations weight3_fops = {

.owner = THIS_MODULE,
.open = weight3_open,
.read = weight3_read,
.release = weight3_release,
};

static struct file_operations weight4_fops = {

.owner = THIS_MODULE,
.open = weight4_open,
.read = weight4_read,
.release = weight4_release,

};


static struct cdev weight_dev;
static struct cdev weight2_dev;
static struct cdev weight3_dev;
static struct cdev weight4_dev;

static struct class *weight_class;

static int __init weight_init(void){
int result;
dev_t dev;

printk("enter weight \n");
if(major){
     dev=MKDEV(major,0);
     result = register_chrdev_region(dev, 1, "weight");
     }

else {
     result = alloc_chrdev_region(&dev, 0, 1, "weight");
     major = MAJOR(dev);
						
     } 



/*weight1 add*/
    cdev_init(&weight_dev, &weight_fops);
    result = cdev_add(&weight_dev, dev, 1);

/*weight2 add*/
    dev=MKDEV(major,2);
    register_chrdev_region(dev, 1, "weight");
    cdev_init(&weight2_dev, &weight2_fops);
    result = cdev_add(&weight2_dev, dev, 1);

/*weight3 add*/
    dev=MKDEV(major,3);
    register_chrdev_region(dev, 1, "weight");
    cdev_init(&weight3_dev, &weight3_fops);
    result = cdev_add(&weight3_dev, dev, 1);



/*weight4 add*/
    dev=MKDEV(major,4);
    register_chrdev_region(dev, 1, "weight");
    cdev_init(&weight4_dev, &weight4_fops);
    result = cdev_add(&weight4_dev, dev, 1); 


    weight_class=class_create(THIS_MODULE, "weight");

    device_create(weight_class,NULL,MKDEV(major,0),NULL,"threesboy_weight1");
    device_create(weight_class,NULL,MKDEV(major,2),NULL,"threesboy_weight2");
    device_create(weight_class,NULL,MKDEV(major,3),NULL,"threesboy_weight3");
    device_create(weight_class,NULL,MKDEV(major,4),NULL,"threesboy_weight4");

    return 0;
}





static int __exit weight_exit(void){

    printk("exit weight \n");

    device_destroy(weight_class, MKDEV(major,0));
    device_destroy(weight_class, MKDEV(major,2));
    device_destroy(weight_class, MKDEV(major,3));
    device_destroy(weight_class, MKDEV(major,4));
    
    class_destroy(weight_class);
    
    cdev_del(&weight_dev);
    unregister_chrdev_region(MKDEV(major,0), 1);
    
    cdev_del(&weight2_dev);
    unregister_chrdev_region(MKDEV(major,2), 1);

    cdev_del(&weight3_dev);
    unregister_chrdev_region(MKDEV(major,3), 1);
    cdev_del(&weight4_dev);
    unregister_chrdev_region(MKDEV(major,4), 1);

    return 0;
}


module_init(weight_init);
module_exit(weight_exit);
