
/*author: xiangzizhou ,versions: wbed1.0 */
/*ds1302 sensor 4 individual*/

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
#include "ds1302_rw.h"
#include <linux/gpio.h>
#include <linux/delay.h>
#include <linux/io.h>
#include <linux/of_gpio.h>
#include <asm/rtc.h>

MODULE_LICENSE("Dual BSD/GPL");
/*声明是开源的，没有内核版本限制*/



#define IMX_GPIO_NR(bank, nr)		(((bank) - 1) * 32 + (nr))

#define GPIO3_18 IMX_GPIO_NR(3, 18) 
#define GPIO3_16 IMX_GPIO_NR(3, 16) 
#define GPIO3_23 IMX_GPIO_NR(3, 23) 

#define RST   GPIO3_18
#define sclk  GPIO3_16 
#define dat   GPIO3_23 

static atomic_t ds1302_atomic = ATOMIC_INIT(1); 
static spinlock_t ds1302_lock;
static int major;
static int ds1302_open (struct inode *inode, struct file *file){
  int ret;
  if(!atomic_dec_and_test(&ds1302_atomic)){
     atomic_inc(&ds1302_atomic);
     return -EBUSY;

    }
  spin_lock_init(&ds1302_lock);
  gpio_request_one(RST,GPIOF_OUT_INIT_LOW,"fec-phy-reset");
  gpio_request_one(sclk,GPIOF_OUT_INIT_LOW,"fec-phy-reset");
  gpio_request_one(dat,GPIOF_OUT_INIT_LOW,"fec-phy-reset");
  
  printk("ds1302 open\n");


      return 0; //this return important if not appear printf luanma	
}

/*ds1302 read to user*/
static ssize_t ds1302_read(struct file *filp, char __user *buf,size_t count, loff_t *f_pos){
     static unsigned int dataread[6];
     static unsigned int sec,min,hour,day,month,year;
     static unsigned int sec_unit,min_unit,hour_unit,day_unit,month_unit,year_unit;
     static unsigned int sec_shi,min_shi,hour_shi,day_shi,month_shi,year_shi;
     static struct ds1302_readtime{
     unsigned int sec;
     unsigned int min;
     unsigned int hour;
     unsigned int day;
     unsigned int month;
     unsigned int year;
     };
     spin_lock(&ds1302_lock);  
     static struct ds1302_readtime readtime;

      sec=read_DS1302(READ_SECOND);
      min=read_DS1302(READ_MINUTE);
      hour=read_DS1302(READ_HOUR);
      day=read_DS1302(READ_DAY);
      month=read_DS1302(READ_MONTH);
      year=read_DS1302(READ_YEARS);
      /*sec*/
      sec_unit=sec&0x0f;
      sec_shi =sec>>4;
      sec_shi=sec_shi*10;
      sec=sec_shi+sec_unit; 
      readtime.sec=sec;
      /*min*/
      min_unit=min&0x0f;
      min_shi =min>>4;
      min_shi =min_shi*10;
      min=min_shi+min_unit;
      readtime.min=min;
      /*hour*/
      hour_unit=hour&0x0f;
      hour_shi =hour>>4;
      hour_shi =hour_shi*10;
      hour=hour_shi+hour_unit;
      readtime.hour=hour;
      /*day*/
      day_unit=day&0x0f;
      day_shi =day>>4;
      day_shi =day_shi*10;
      day = day_shi+day_unit;
      readtime.day=day;
      /*month*/
      month_unit=month&0x0f;
      month_shi =month>>4;
      month_shi =month_shi*10;
      month=month_shi+month_unit;
      readtime.month=month;
      /*year*/
      year_unit=year&0x0f;
      year_shi =year>>4;
      year_shi =year_shi*10;
      year=year_shi+year_unit;
      readtime.year=year;
      spin_unlock(&ds1302_lock);
      if(copy_to_user(buf,&readtime, sizeof(readtime))!=0){

            return -EFAULT;

       }

      return 1;
}

static ssize_t ds1302_write(struct file *filp,const char __user *buf,size_t size ,loff_t *ppos){
	
      static unsigned char bcd8421[60]={0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x10,
                                   0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x20,0x21,
	                           0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,0x30,0x31,0x32,0x33,0x34,
                                   0x35,0x36,0x37,0x38,0x39,0x40,0x41,0x42,0x43,0x44,0x45,0x46,0x47,0x48,
                                   0x49,0x50,0x51,0x52,0x53,0x54,0x55,0x56,0x57,0x58,0x59};

      static  struct ds1302_time{
      unsigned int sec;
      unsigned int min;
      unsigned int hour;
      unsigned int day;
      unsigned int month;
      unsigned int year;
      };
      struct ds1302_time ds1302_write_time;
      printk("enter ds1302_write\n");
	if(copy_from_user(&ds1302_write_time,buf,size)!=0){
	  printk("write ds1302 dtat to kernel failed!\n");
	}

 //   printk(" timesec = %d timemin = %d timehour = %d  timeday= %d timemonth= %d timeyear=%d\n",ds1302_write_time.sec,ds1302_write_time.min,ds1302_write_time.hour,ds1302_write_time.day,ds1302_write_time.month,ds1302_write_time.year);
     spin_lock(&ds1302_lock);
       	init_ds1302(bcd8421[ds1302_write_time.sec],bcd8421[ds1302_write_time.min],bcd8421[ds1302_write_time.hour],bcd8421[ds1302_write_time.day],bcd8421[ds1302_write_time.month],bcd8421[ds1302_write_time.year]);
     spin_unlock(&ds1302_lock);
return 0;

} 




int ds13021_release(struct inode *inode, struct file *file){
        gpio_free(GPIO3_18);
        gpio_free(GPIO3_16);
        gpio_free(GPIO3_23);
	atomic_inc(&ds1302_atomic);
        printk("close ds13021\n");
        return 0;

}









static struct file_operations ds1302_fops = {

.owner = THIS_MODULE,
.open = ds1302_open,
.read = ds1302_read,
.write = ds1302_write,
.release = ds13021_release,

};


 














static struct cdev ds1302_dev;
static struct class *ds1302_class;

static int ds1302_init(void){
int result;
dev_t dev;

printk("enter ds1302 \n");
if(major){
     dev=MKDEV(major,0);
     result = register_chrdev_region(dev, 1, "ds1302");
     }

else {
     result = alloc_chrdev_region(&dev, 0, 1, "ds1302");
     major = MAJOR(dev);
						
     } 



/*ds1302 add*/
    cdev_init(&ds1302_dev, &ds1302_fops);
    result = cdev_add(&ds1302_dev, dev, 1);



    ds1302_class=class_create(THIS_MODULE, "ds1302");

    device_create(ds1302_class,NULL,MKDEV(major,0),NULL,"threesboy_RTC");

    return 0;
}





static int ds1302_exit(void){

    printk("exit ds1302 \n");

    device_destroy(ds1302_class, MKDEV(major,0));
    
    class_destroy(ds1302_class);
    
    cdev_del(&ds1302_dev);
    unregister_chrdev_region(MKDEV(major,0), 1);
    

    return 0;
}


module_init(ds1302_init);
module_exit(ds1302_exit);
