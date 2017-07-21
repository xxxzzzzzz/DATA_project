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
//
///*包含函数device_create 结构体class等头文件*/
#include <linux/device.h>
#include <linux/gpio.h>
#include <linux/delay.h>
#include <linux/io.h>
#include <linux/of_gpio.h>
MODULE_LICENSE("Dual BSD/GPL");

#define IMX_GPIO_NR(bank, nr)           (((bank) - 1) * 32 + (nr))


#define X9313_uid_gpio IMX_GPIO_NR(4, 22)  //X9313_UID     
#define X9313_CS_gpio IMX_GPIO_NR(4, 23) //X9313_CS      
#define X9313_INC_gpio IMX_GPIO_NR(4, 21) //X9313_INC
#define RGB_led_RED IMX_GPIO_NR(4, 5) //LED_RED
#define RGB_led_GREEN IMX_GPIO_NR(7, 11) //LED_GREEN
#define RGB_led_BLUE IMX_GPIO_NR(1, 7) //LED_BLUE
static unsigned long led_array[3]={
	RGB_led_RED,
	RGB_led_GREEN,
	RGB_led_BLUE,
        };


static int major=0;

static int RGBled_wbed_open (struct inode *inode, struct file *file){
int ret;

printk("enter RGBled_wbed_open\n");
//................X9313_uid_gpio.........init
ret = gpio_request_one(X9313_uid_gpio,GPIOF_OUT_INIT_HIGH, "fec-phy-reset");
if(ret){printk("gpio X9313_uid_gpio request failed\n");}
else {printk("gpio X9313_uid_gpio  request success\n");}
//................X9313_CS_gpio.........init
ret = gpio_request_one(X9313_CS_gpio,GPIOF_OUT_INIT_HIGH, "fec-phy-reset");
if(ret){printk("gpio X9313_CS_gpio request failed\n");}
else {printk("gpio X9313_CS_gpio  request success\n");}
//................X9313_INC_gpio.........init
ret = gpio_request_one(X9313_INC_gpio,GPIOF_OUT_INIT_HIGH, "fec-phy-reset");
if(ret){printk("gpio X9313_INC_gpio request failed\n");}
else {printk("gpio  X9313_INC_gpio  request success\n");}

//................RGB_led_RED.........init
ret = gpio_request_one(RGB_led_RED,GPIOF_OUT_INIT_HIGH, "fec-phy-reset");
if(ret){printk("gpio RGB_led_RED request failed\n");}
else {printk("gpio  RGB_led_RED  request success\n");}

//................RGB_led_GREEN.........init
ret = gpio_request_one(RGB_led_GREEN,GPIOF_OUT_INIT_HIGH, "fec-phy-reset");
if(ret){printk("gpio RGB_led_GREEN request failed\n");}
else {printk("gpio  RGB_led_GREEN  request success\n");}

//................RGB_led_BLUE.........init
ret = gpio_request_one(RGB_led_BLUE,GPIOF_OUT_INIT_HIGH, "fec-phy-reset");
if(ret){printk("gpio RGB_led_BLUE request failed\n");}
else {printk("gpio  RGB_led_BLUE  request success\n");}

return 0;
}


static long RGBled_wbed_ioctl(struct file *file, unsigned int cmd, unsigned long arg){
printk("enter RGBled_wbed_ioctl\n");

switch(cmd){
        case 0:gpio_set_value(led_array[arg],0);break;//RGB_led_on

	case 1:gpio_set_value(led_array[arg],1);break;//RGB_led_off
	default:
	
	return -EINVAL;

}

return 0;
}

static ssize_t RGB_lumin_write(struct file *filp, const char __user *buf, size_t count, loff_t *f_pos){
	int i,ret;
	int val,data;
	ret=copy_from_user(&val, buf, count);
	if(ret){printk("RGB write failed\n");}
	else
		printk("enter RGB_lumin_write data = %d\n",val);
	gpio_set_value(X9313_CS_gpio,0); //select X9313 chip
	gpio_set_value(X9313_uid_gpio,0);//reduce voltage
	for(i=100;i>0;i--)
	{
		gpio_set_value(X9313_INC_gpio,0);
		gpio_set_value(X9313_INC_gpio,1);
	
	}
				   	   
	gpio_set_value(X9313_uid_gpio,1);//inc voltage
	
	for(data=val;data>0;data--)
	{
		gpio_set_value(X9313_INC_gpio,0);
		gpio_set_value(X9313_INC_gpio,1);

	}		
	
	
	gpio_set_value(X9313_CS_gpio,1); //no select X9313 chip  


return 0;
}

int RGBled_wbed_release(struct inode *inode, struct file *file){
gpio_free(X9313_uid_gpio);
gpio_free(X9313_CS_gpio);
gpio_free(X9313_INC_gpio);
gpio_free(RGB_led_RED);
gpio_free(RGB_led_GREEN);
gpio_free(RGB_led_BLUE);

printk("enter RGBled_wbed_close\n");
return 0;
}


static struct file_operations led_wbed_fops = {

   .owner = THIS_MODULE,
   .open = RGBled_wbed_open,
   .write = RGB_lumin_write,
   .unlocked_ioctl = RGBled_wbed_ioctl,
   .release = RGBled_wbed_release,


};


static struct cdev  RGBled_wbed_dev;
static struct class *RGB_wbed_class;

static int RGBled_init(void){
  int result;
  dev_t dev;

  printk("enter RGBled_init\n");

  if(major){
   dev=MKDEV(major,0);
   result = register_chrdev_region(dev, 1, "RGB_leds");
		       
           }

   else {
   result = alloc_chrdev_region(&dev, 0, 1, "RGB_leds");
   major = MAJOR(dev);

        }
             
  cdev_init(&RGBled_wbed_dev,&led_wbed_fops);
  result = cdev_add(&RGBled_wbed_dev,dev,1);
  RGB_wbed_class=class_create(THIS_MODULE, "RGBled");
  device_create(RGB_wbed_class,NULL,MKDEV(major,0),NULL,"RGB_led");

   return 0;
}

static int RGBled_exit(void){

   printk("exit RGBled_init\n");
   device_destroy(RGB_wbed_class, MKDEV(major,0));
   class_destroy(RGB_wbed_class);
   cdev_del(&RGBled_wbed_dev);
   unregister_chrdev_region(MKDEV(major,0), 1);

	return 0;   

}



	



module_init(RGBled_init);
module_exit(RGBled_exit);
