
/*author: xiangzizhou ,button*/

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
#include <asm/irq.h>
#include <linux/irqreturn.h>
#include <linux/interrupt.h>
#include <linux/wait.h>　
#include <linux/sched.h>
#include <mach/pincontrol.h>

MODULE_LICENSE("Dual BSD/GPL");
/*声明是开源的，没有内核版本限制*/


static int irq_status = 0;

static irqreturn_t buttons_irq(int irq,void* dev_id){
	printk("enter buttons irq process\n");
	if(get_io_irq_status(1,4)==0) //再次判断指定引脚中断是否确实发生
	{
		irq_status = get_io_irq_status(0,4);//返回引脚发生中断的状态，其实可以不要这句，正式程序不写这句
		printk("irq_status = %d\n",irq_status);
		
	}
	io_irq_clr(1,4);//清除中断标志位
	return IRQ_RETVAL(IRQ_HANDLED);
}


static int __init button_init(void){
	int irqres;
	printk("enter button \n");
	io_irq_enable_edge(1, 4, GPIO_IRQ_EDGE_FALLING);
    	irqres = request_irq((INT_GPIO0+1/4),buttons_irq,IRQF_TRIGGER_FALLING,"key",NULL);
	return 0;
}


static void __exit button_exit(void){

    	printk("exit button \n");
	io_irq_disable(1, 4); //清除引脚中断功能
	free_irq((INT_GPIO0+1/4),NULL);//清除绑定的中断源
    	
}


module_init(button_init);
module_exit(button_exit);
