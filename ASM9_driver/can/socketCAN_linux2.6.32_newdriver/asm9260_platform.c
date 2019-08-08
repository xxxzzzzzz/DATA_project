/***********************************************
 *  linux/drivers/net/can/asm9260/asm9260_platform.c
 *  Copyright (C) 2014 Alpscale
 *
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/interrupt.h>
#include <linux/netdevice.h>
#include <linux/delay.h>
#include <linux/pci.h>
#include <linux/platform_device.h>
#include <linux/irq.h>
#include <linux/can.h>
#include <mach/pincontrol.h>
#include  "dev.h"
#include <linux/io.h>
#include <mach/io.h>
#include <mach/asm9260_canserial.h>

#include "asm9260.h"
#include "asm92601.h"

#define DRV_NAME "asm9260_platform"


MODULE_AUTHOR("xuling");
MODULE_DESCRIPTION("Socket-CAN driver for ASM9260 on the platform bus");
MODULE_LICENSE("GPL v2");

/*
 * asm9260  can   read
 */
static u8 sp_read_reg(const struct asm9260_priv *priv, int reg)
{
       return  __raw_readb(priv->reg_base +reg);
}

/*
 * asm9260  can   write
 */
static void sp_write_reg(const struct asm9260_priv *priv, int reg, u8 val)
{
      __raw_writeb (val,reg+priv->reg_base) ;
}

/*
 * asm9260  can  probe
 */
static int sp_probe(struct platform_device *pdev)
{
	int err;
	void __iomem *addr;
	struct net_device *dev;
	struct asm9260_priv *priv;
	struct resource  *res_irq;
	struct asm9260_platform_data *pdata;

	pdata = pdev->dev.platform_data;
        if (!pdata) {
		err = -ENODEV;
		goto exit;
	}

	res_irq = &pdev->resource[1];

	addr=pdata->regs;

	dev = alloc_asm9260dev(0);
	if (!dev) {
		err = -ENOMEM;
		goto exit_iounmap;
	}
	priv = netdev_priv(dev);

	dev->irq = res_irq->start;
	priv->irq_flags = res_irq->flags & IRQF_TRIGGER_MASK;
	priv->reg_base = addr;
	priv->can.clock.freq = as3310_readl(HW_SYSPLLCTRL)/as3310_readl(HW_CPUCLKDIV)/as3310_readl(HW_SYSAHBCLKDIV);
	priv->can.clock.freq = priv->can.clock.freq * 1000000 ;
	priv->read_reg = sp_read_reg;
	priv->write_reg = sp_write_reg;
	priv->ocr = pdata->ocr;
	priv->cdr = pdata->cdr;

	dev_set_drvdata(&pdev->dev, dev);

	SET_NETDEV_DEV(dev, &pdev->dev);

	err = register_asm9260dev(dev);

	if (err) {
		dev_err(&pdev->dev, "registering %s failed (err=%d)\n",
			DRV_NAME, err);
		goto exit_free;
	}

	dev_info(&pdev->dev, "%s device registered (reg_base=%p, irq=%d)\n",
		 DRV_NAME, priv->reg_base, dev->irq);
	return 0;
      
 exit_free:
	free_asm9260dev(dev);
 exit_iounmap:
 exit:
	return err;
}

/*
 * asm9260  can   remove
 */
static int sp_remove(struct platform_device *pdev)
{
	struct net_device *dev = dev_get_drvdata(&pdev->dev);
	struct asm9260_priv *priv = netdev_priv(dev);
	struct resource *res;

	unregister_asm9260dev(dev);
	dev_set_drvdata(&pdev->dev, NULL);

	if (priv->reg_base)
		iounmap(priv->reg_base);

	res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	release_mem_region(res->start, resource_size(res));

	free_asm9260dev(dev);

	return 0;
}

/*
 * asm9260  can   platform_driver
 */
static struct platform_driver sp_driver = {
	.probe = sp_probe,
	.remove = sp_remove,
	.driver = {
		.name = DRV_NAME,
		.owner = THIS_MODULE,
	},
};

/*
 * asm9260  can mode init
 */
static int __init sp_init(void)
{
        as3310_writel(1<<28,HW_PRESETCTRL0+8);      
        as3310_writel(1<<27, HW_PRESETCTRL0+4);     //reset CAN0 module
        as3310_writel(1<<28, HW_PRESETCTRL0+4);     //reset CAN1 module
        as3310_writel(1<<27, HW_AHBCLKCTRL0+4);     //enable CAN0 timer
        as3310_writel(1<<28, HW_AHBCLKCTRL0+4);     //enable CAN0 timer
//        as3310_writel(0x20000038, HW_ICOLL_VBASE);
//        as3310_writel(0x00070000, HW_ICOLL_CTRL); 
//        as3310_writel(0X00000400, HW_ICOLL_PRIORITY14);
        as3310_writeb(0x02,REG_OCR+0x80050000);

	return platform_driver_register(&sp_driver);
}

/*
 * asm9260  can mode exit
 */
static void __exit sp_exit(void)
{
        free_irq(57,NULL);
	platform_driver_unregister(&sp_driver);
}

module_init(sp_init);
module_exit(sp_exit);
