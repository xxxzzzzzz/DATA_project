
/*author: xiangzizhou ,versions: wbed1.0 */
/*mcp2515 sensor 4 individual*/

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
#include <linux/gpio.h>
#include <linux/delay.h>
#include <linux/io.h>
#include <linux/of_gpio.h>
#include "mcp2515_spi.h" 

MODULE_LICENSE("Dual BSD/GPL");
/*声明是开源的，没有内核版本限制*/


#define IMX_GPIO_NR(bank, nr)		(((bank) - 1) * 32 + (nr))

#define GPIO4_06 IMX_GPIO_NR(4, 6) 
#define GPIO4_08 IMX_GPIO_NR(4, 8) 
#define GPIO4_07 IMX_GPIO_NR(4, 7) 
#define GPIO4_09 IMX_GPIO_NR(4, 9) 
#define GPIO4_11 IMX_GPIO_NR(4, 11) 

#define SCK   GPIO4_06
#define MISO  GPIO4_08
#define MOSI  GPIO4_07
#define NSS   GPIO4_09
#define INT   GPIO4_11

static DEFINE_MUTEX(rtc_mutex);

static atomic_t mcp2515_atomic = ATOMIC_INIT(1); 
static int major;
static int mcp2515_open (struct inode *inode, struct file *file){
	
	static int ret;
	static unsigned char i;

	printk("mcp2515 open\n");


      	ret = gpio_request_one(SCK,GPIOF_OUT_INIT_HIGH, "fec-phy-reset");
      	if(ret){printk("mcp2515 SCK request failed\n");}
      	else {printk("mcp2515 SCK  request success\n");} 
      	ret = gpio_request_one(MISO,GPIOF_IN, "fec-phy-reset");
      	if(ret){printk("mcp2515 MISO request failed\n");}
      	else {printk("mcp2515 MISO  request success\n");} 
      	ret = gpio_request_one(MOSI,GPIOF_OUT_INIT_HIGH, "fec-phy-reset");
      	if(ret){printk("mcp2515 MOSI request failed\n");}
      	else {printk("mcp2515 MOSI  request success\n");} 
      	ret = gpio_request_one(NSS,GPIOF_OUT_INIT_HIGH, "fec-phy-reset");
      	if(ret){printk("mcp2515 NSS request failed\n");}
      	else {printk("mcp2515 NSS  request success\n");} 
      	ret = gpio_request_one(INT,GPIOF_IN, "fec-phy-reset");
      	if(ret){printk("mcp2515 INT request failed\n");}
      	else {printk("mcp2515 INT  request success\n");} 
	

	MCP2515_CAN_INIT();
	MCP2515_WriteByte(0x31,0x00);
	MCP2515_WriteByte(0x32,0x08);
	MCP2515_WriteByte(0x33,0x11);
	MCP2515_WriteByte(0x34,0xff);
	MCP2515_WriteByte(0x35,0x08);
	MCP2515_WriteByte(0x36,0x11);
	MCP2515_WriteByte(0x37,0x11);
	MCP2515_WriteByte(0x38,0x11);
	MCP2515_WriteByte(0x39,0x11);
	MCP2515_WriteByte(0x3a,0xff);
	MCP2515_WriteByte(0x3b,0xff);
	MCP2515_WriteByte(0x3c,0xff);
	MCP2515_WriteByte(0x3d,0xff);
	while(1){
	CAN_set_var(0x30,0x0b,0x0b);	
	
	do
	{
	 i=MCP2515_ReadByte(0x30);
	 i=i&0x08;
	}while(i);

	mdelay(500);
	}
	
	return 0; //this return important if not appear printf luanma	
}

/*mcp2515 read to user*/
static ssize_t mcp2515_read(struct file *filp, char __user *buf,size_t count, loff_t *f_pos){

     return 0; 
}




static ssize_t mcp2515_write(struct file *filp,const char __user *buf,size_t size ,loff_t *ppos){
   return 0;

} 




int mcp25151_release(struct inode *inode, struct file *file){
        gpio_free(SCK);
        gpio_free(MOSI);
        gpio_free(MISO);
        gpio_free(NSS);
        gpio_free(INT);

        printk("close mcp25151\n");
        return 0;

}









static struct file_operations mcp2515_fops = {

.owner = THIS_MODULE,
.open = mcp2515_open,
.read = mcp2515_read,
.write = mcp2515_write,
.release = mcp25151_release,

};


 














static struct cdev mcp2515_dev;
static struct class *mcp2515_class;

static int mcp2515_init(void){
int result;
dev_t dev;

printk("enter mcp2515 \n");
if(major){
     dev=MKDEV(major,0);
     result = register_chrdev_region(dev, 1, "mcp2515");
     }

else {
     result = alloc_chrdev_region(&dev, 0, 1, "mcp2515");
     major = MAJOR(dev);
						
     } 



	/*mcp2515 add*/
    cdev_init(&mcp2515_dev, &mcp2515_fops);
    result = cdev_add(&mcp2515_dev, dev, 1);



    mcp2515_class=class_create(THIS_MODULE, "mcp2515");

    device_create(mcp2515_class,NULL,MKDEV(major,0),NULL,"mcp2515_can");

    return 0;
}





static int mcp2515_exit(void){

    printk("exit mcp2515 \n");

    device_destroy(mcp2515_class, MKDEV(major,0));
    
    class_destroy(mcp2515_class);
    
    cdev_del(&mcp2515_dev);
    unregister_chrdev_region(MKDEV(major,0), 1);
    

    return 0;
}


module_init(mcp2515_init);
module_exit(mcp2515_exit);
