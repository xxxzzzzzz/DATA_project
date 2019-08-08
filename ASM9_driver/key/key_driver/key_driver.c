#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/interrupt.h>
#include <linux/irq.h>
#include <linux/sched.h>
#include <linux/pm.h>
#include <linux/slab.h>
#include <linux/sysctl.h>
#include <linux/proc_fs.h>
#include <linux/delay.h>
#include <linux/platform_device.h>
#include <linux/input.h>
#include <linux/gpio_keys.h>
#include <linux/workqueue.h>
#include <linux/gpio.h>
#include <linux/spinlock.h>
#include <mach/pincontrol.h> 
#include <asm/irq.h> 
#include <asm/io.h>
MODULE_LICENSE("Dual BSD/GPL");

//#define DEBUG
struct key_GPIO_definit  
{                                                                                                                                                       
	int key_group;   
	int key_gpio_pin;
};


static struct key_GPIO_definit *plat_key_gpio;

/* creat input_dev struct*/
static struct input_dev *AP_switch_dev;
static struct timer_list AP_switch_timer;//定义一个定时器，用于按键消抖功能
static unsigned char kerneltime = 0;

static irqreturn_t AP_switchs_irq(int irq,void* dev_id){
	kerneltime = 1;
	printk("enter irq\n");
	/*10ms后启动定时器*/ 
	io_irq_mask(plat_key_gpio->key_group, plat_key_gpio->key_gpio_pin);
	io_irq_clr(plat_key_gpio->key_group, plat_key_gpio->key_gpio_pin);//清除中断标志位
	mod_timer(&AP_switch_timer,jiffies+HZ/100);
	return IRQ_RETVAL(IRQ_HANDLED);
}

/*定时器中断处理函数*/
static void AP_switchs_timer_function(unsigned long data){
	if(kerneltime==0)
		return ;
	//if(get_io_irq_status(plat_key_gpio->key_group, plat_key_gpio->key_gpio_pin)==0)
	mdelay(10);
	if(read_GPIO(plat_key_gpio->key_group, plat_key_gpio->key_gpio_pin)==0)
	{
#ifdef DEBUG
		printk("if irq_status \n");
#endif
		input_event(AP_switch_dev,EV_KEY,KEY_L,1);
		input_sync(AP_switch_dev);
		io_irq_enable_externIO(plat_key_gpio->key_group, plat_key_gpio->key_gpio_pin,GPIO_IRQ_EDGE_FALLING);
		kerneltime = 0;
	}
	else{
#ifdef DEBUG
		printk("else irq_status \n");
#endif
		input_event(AP_switch_dev,EV_KEY,KEY_L,0);
		input_sync(AP_switch_dev);
		io_irq_enable_externIO(plat_key_gpio->key_group, plat_key_gpio->key_gpio_pin,GPIO_IRQ_EDGE_FALLING);
		kerneltime = 0;
	}
}

static int __devinit key_probe(struct platform_device *pdev)
{	
	int irqres;
	int ret;
	plat_key_gpio = pdev->dev.platform_data;
#ifdef DEBUG
	printk("key gpio_group = %d\n",plat_key_gpio->key_group);
	printk("key gpio_pin = %d\n",plat_key_gpio->key_gpio_pin);
#endif
	set_pin_mux(plat_key_gpio->key_group, plat_key_gpio->key_gpio_pin,0);
   	
	io_irq_enable_externIO(plat_key_gpio->key_group, plat_key_gpio->key_gpio_pin,GPIO_IRQ_EDGE_FALLING);

	AP_switch_dev=input_allocate_device();
	if(!AP_switch_dev)
		printk("input_allocate_device erro\n");

	set_bit(EV_KEY,AP_switch_dev->evbit);
	set_bit(EV_REP, AP_switch_dev->evbit);
	set_bit(KEY_L, AP_switch_dev->keybit);

	ret=input_register_device(AP_switch_dev);
	if(ret){
		printk("input_register_device failed\n");
		}
    
	init_timer(&AP_switch_timer); 
    	AP_switch_timer.function = AP_switchs_timer_function;
    	add_timer(&AP_switch_timer);
	
	irqres = request_irq((INT_GPIO0+plat_key_gpio->key_group/4),AP_switchs_irq,IRQF_TRIGGER_FALLING|IRQ_TYPE_EDGE_RISING,"AP_sw",NULL); //IRQF_TRIGGER_RISING
	if(irqres)
		printk("request irq failed ..\n");


	
#ifdef DEBUG
	printk("input_AP_keys_init\n");
#endif
	return	0;
}
static int key_remove(struct platform_device *pdev)
{
	io_irq_disable(plat_key_gpio->key_group, plat_key_gpio->key_gpio_pin); //清除引脚中断功能
	free_irq((INT_GPIO0+plat_key_gpio->key_group/4),NULL);//清除绑定的中断源
	del_timer(&AP_switch_timer);
	input_unregister_device(AP_switch_dev);
	input_free_device(AP_switch_dev);
	return 0;
}
static struct platform_driver key_platform_driver = {
	.probe= key_probe,
	.remove= key_remove,
	.driver= {
	       .name= "key_driver_device",
	       .owner= THIS_MODULE,
	},


};



static int __init input_keys_init(void){
	
	platform_driver_register(&key_platform_driver);
#ifdef DEBUG
	printk("input keys init...\n");
#endif
	return 0;
}



static void __exit input_keys_exit(void){
		
	platform_driver_unregister(&key_platform_driver);
#ifdef DEBUG
	printk("input keys exit...\n");
#endif
	

}



module_init(input_keys_init);
module_exit(input_keys_exit);

