#include <linux/module.h>
#include <linux/version.h>
#include <linux/init.h>
#include <linux/interrupt.h>
#include <linux/irq.h>
#include <linux/input.h>
#include <linux/gpio_keys.h>
#include <mach/pincontrol.h>

#define ASM_KEY_GROUP 17
#define ASM_KEY_IO_PIN 6

static struct input_dev *buttons_dev;
static struct timer_list buttons_timer;

static irqreturn_t buttons_irq(int irq, void *dev_id)
{
	// io_irq_mask(ASM_KEY_GROUP, ASM_KEY_IO_PIN);
	io_irq_clr(ASM_KEY_GROUP, ASM_KEY_IO_PIN);

	mod_timer(&buttons_timer, jiffies + HZ / 100);
	return IRQ_RETVAL(IRQ_HANDLED);
}

static void buttons_timer_function(unsigned long data)
{
	unsigned int pinval;

	pinval = read_GPIO(ASM_KEY_GROUP, ASM_KEY_IO_PIN);
	if (pinval)
	{
		input_event(buttons_dev, EV_KEY, KEY_ENTER, 0);
		input_sync(buttons_dev);
	}
	else
	{
		input_event(buttons_dev, EV_KEY, KEY_ENTER, 1);
		input_sync(buttons_dev);
	}
	io_irq_enable_externIO(ASM_KEY_GROUP, ASM_KEY_IO_PIN, GPIO_IRQ_EDGE_BOTH);
}

static int buttons_init(void)
{
	int rc;

	buttons_dev = input_allocate_device();
	if (!buttons_dev)
		printk("input_allocate_device failure\n");

	buttons_dev->name = "AP_BTN";

	set_bit(EV_KEY, buttons_dev->evbit);
	// set_bit(EV_REP, buttons_dev->evbit);

	set_bit(KEY_ENTER, buttons_dev->keybit);

	rc = input_register_device(buttons_dev);
	if (rc)
		printk("input_register_device failure\n");

	init_timer(&buttons_timer);
	buttons_timer.function = buttons_timer_function;
	add_timer(&buttons_timer);

	rc = request_irq((INT_GPIO0 + ASM_KEY_GROUP / 4), buttons_irq, IRQ_TYPE_EDGE_BOTH, "AP_BTN_IRQ", NULL);
	if (rc)
		printk("request_irq failure\n");

	set_pin_mux(ASM_KEY_GROUP, ASM_KEY_IO_PIN, 0);
	io_irq_enable_externIO(ASM_KEY_GROUP, ASM_KEY_IO_PIN, GPIO_IRQ_EDGE_BOTH);

	printk("buttons_init success\n");
	return 0;
}

static void buttons_exit(void)
{
	free_irq((INT_GPIO0 + ASM_KEY_GROUP / 4), NULL);

	del_timer(&buttons_timer);
	input_unregister_device(buttons_dev);
	input_free_device(buttons_dev);
	printk("buttons_exit success\n");
}

module_init(buttons_init);
module_exit(buttons_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("CdataBox company");
MODULE_DESCRIPTION("This is a mod which provide AP button up/down detection on /dev/eventX");
MODULE_VERSION("Verson 1.0");