/***********************************************
 *  linux/drivers/net/can/asm9260/asm9260.c
 *  Copyright (C) 2014 Alpscale
 *
 */

#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/types.h>
#include <linux/fcntl.h>
#include <linux/interrupt.h>
#include <linux/ptrace.h>
#include <linux/string.h>
#include <linux/errno.h>
#include <linux/netdevice.h>
#include <linux/if_arp.h>
#include <linux/if_ether.h>
#include <linux/skbuff.h>
#include <linux/delay.h>
#include <linux/can.h>
#include  "dev.h"
#include <linux/can/error.h>

#include "asm9260.h"

#define DRV_NAME "asm9260"

MODULE_AUTHOR("xuling");
MODULE_LICENSE("Dual BSD/GPL");
MODULE_DESCRIPTION(DRV_NAME "CAN netdevice driver");

/*
 * asm9260  can   bittiming const
 */
static struct can_bittiming_const asm9260_bittiming_const = {
	.name = DRV_NAME,
	.tseg1_min = 1,
	.tseg1_max = 16,
	.tseg2_min = 1,
	.tseg2_max = 8,
	.sjw_max = 4,
	.brp_min = 1,
	.brp_max = 64,
	.brp_inc = 1,
};

/*
 * asm9260  can mode probe 
 */
static int asm9260_probe_chip(struct net_device *dev)
{
	struct asm9260_priv *priv = netdev_priv(dev);

	if (priv->reg_base && (priv->read_reg(priv, 0) == 0xFF)) {
		printk(KERN_INFO "%s: probing @0x%lX failed\n",
		       DRV_NAME, dev->base_addr);
		return 0;
	}
	return -1;
}

/*
 * asm9260  chip  in reset mode 
 */
static void set_reset_mode(struct net_device *dev)
{
	struct asm9260_priv *priv = netdev_priv(dev);
	unsigned char status = priv->read_reg(priv, REG_MOD);
	int i;

	/* disable interrupts */
	priv->write_reg(priv, REG_IER, IRQ_OFF);

	for (i = 0; i < 100; i++) {
		/* check reset bit */
		if (status & MOD_RM) {
			priv->can.state = CAN_STATE_STOPPED;
			return;
		}

		priv->write_reg(priv, REG_MOD, MOD_RM);	/* reset chip */
		udelay(10);
		status = priv->read_reg(priv, REG_MOD);
	}

	dev_err(dev->dev.parent, "setting asm9260 into reset mode failed!\n");
}

/*
 * asm9260  chip  in normal mode
 */
static void set_normal_mode(struct net_device *dev)
{
	struct asm9260_priv *priv = netdev_priv(dev);
	unsigned char status = priv->read_reg(priv, REG_MOD);
	int i;

	for (i = 0; i < 100; i++) {
		/* check reset bit */
		if ((status & MOD_RM) == 0) {
			priv->can.state = CAN_STATE_ERROR_ACTIVE;
			/* enable all interrupts */
			priv->write_reg(priv, REG_IER, 0x7b);
			return;
		}

		/* set chip to normal mode */
		priv->write_reg(priv, REG_MOD, 0x00);
		udelay(10);
		status = priv->read_reg(priv, REG_MOD);
	}

	dev_err(dev->dev.parent, "setting asm9260 into normal mode failed!\n");
}

/*
 * asm9260 mode start
 */
static void asm9260_start(struct net_device *dev)
{
	struct asm9260_priv *priv = netdev_priv(dev);

	/* leave reset mode */
	if (priv->can.state != CAN_STATE_STOPPED)
		set_reset_mode(dev);

	/* Clear error counters and error code capture */
	priv->write_reg(priv, REG_TXERR, 0x0);
	priv->write_reg(priv, REG_RXERR, 0x0);
	priv->read_reg(priv, REG_ECC);

	/* leave reset mode */
	set_normal_mode(dev);
}

/*
 * asm9260 mode set
 */
static int asm9260_set_mode(struct net_device *dev, enum can_mode mode)
{
	struct asm9260_priv *priv = netdev_priv(dev);

	if (!priv->open_time)
		return -EINVAL;

	switch (mode) {
	case CAN_MODE_START:
		asm9260_start(dev);
		if (netif_queue_stopped(dev))
			netif_wake_queue(dev);
		break;

	default:
		return -EOPNOTSUPP;
	}

	return 0;
}

/* 
 * set asm9260 can bit rate.
 */

static int asm9260_set_bittiming(struct net_device *dev)
{
	struct asm9260_priv *priv = netdev_priv(dev);
	struct can_bittiming *bt = &priv->can.bittiming;
        u8 btr0, btr1,btr2;

	btr0 = ((bt->brp - 1) & 0x3f) | (((bt->sjw - 1) & 0x3) << 6);
	btr1 = ((bt->prop_seg + bt->phase_seg1 - 1) & 0xf) |
		(((bt->phase_seg2 - 1) & 0x7) << 4);
	btr2 = (((bt->brp-1)&0x3c0)>>6);
	if (priv->can.ctrlmode & CAN_CTRLMODE_3_SAMPLES)
		btr1 |= 0x80;


	dev_info(dev->dev.parent,
		 "setting BTR0=0x%02x BTR1=0x%02x BTR2=0x%02x\n", btr0, btr1,btr2);

	priv->write_reg(priv, REG_BTR0, btr0);
	priv->write_reg(priv, REG_BTR1, btr1);
	priv->write_reg(priv, REG_BTR2, btr2);

	return 0;
}

/*
 * initialize asm9260 chip:
 *   - reset chip
 *   - set output mode
 *   - set baudrate
 *   - enable interrupts
 *   - start operating mode
 */
static void chipset_init(struct net_device *dev)
{
	struct asm9260_priv *priv = netdev_priv(dev);

	/* set clock divider and output control register */
	priv->write_reg(priv, REG_CDR, priv->cdr | CDR_PELICAN);

	/* set acceptance filter (accept all) */
	priv->write_reg(priv, REG_ACCC0, 0x00);
	priv->write_reg(priv, REG_ACCC1, 0x00);
	priv->write_reg(priv, REG_ACCC2, 0x00);
	priv->write_reg(priv, REG_ACCC3, 0x00);

	priv->write_reg(priv, REG_ACCM0, 0xFF);
	priv->write_reg(priv, REG_ACCM1, 0xFF);
	priv->write_reg(priv, REG_ACCM2, 0xFF);
	priv->write_reg(priv, REG_ACCM3, 0xFF);

	priv->write_reg(priv, REG_OCR, priv->ocr | OCR_MODE_NORMAL);
}

/*
 * transmit a CAN message
 * message layout in the sk_buff should be like this:
 * xx xx xx xx	 ff	 ll   00 11 22 33 44 55 66 77
 * [  can-id ] [flags] [len] [can data (up to 8 bytes]
 */
int asm9260_start_xmit(struct sk_buff *skb, struct net_device *dev)
{
	struct asm9260_priv *priv = netdev_priv(dev);
	struct can_frame *cf = (struct can_frame *)skb->data;
	uint8_t fi;
	uint8_t dlc;
	canid_t id;
	uint8_t dreg;
	int i;

	netif_stop_queue(dev);

	fi = dlc = cf->can_dlc;
	id = cf->can_id;

	if (id & CAN_RTR_FLAG)
		fi |= FI_RTR;

	if (id & CAN_EFF_FLAG) {
		fi |= FI_FF;
		dreg = EFF_BUF;
		priv->write_reg(priv, REG_FI, fi);
		priv->write_reg(priv, REG_ID1, (id & 0x1fe00000) >> (5 + 16));
		priv->write_reg(priv, REG_ID2, (id & 0x001fe000) >> (5 + 8));
		priv->write_reg(priv, REG_ID3, (id & 0x00001fe0) >> 5);
		priv->write_reg(priv, REG_ID4, (id & 0x0000001f) << 3);
	} else {
		dreg = SFF_BUF;
		priv->write_reg(priv, REG_FI, fi);
		priv->write_reg(priv, REG_ID1, (id & 0x000007f8) >> 3);
		priv->write_reg(priv, REG_ID2, (id & 0x00000007) << 5);
	}

	for (i = 0; i < dlc; i++)
		priv->write_reg(priv, dreg++, cf->data[i]);

	dev->trans_start = jiffies;

	can_put_echo_skb(skb, dev, 0);

	priv->write_reg(priv, REG_CMR, CMD_TR);

	return NETDEV_TX_OK;
}
EXPORT_SYMBOL_GPL(asm9260_start_xmit);

/*
 * receive a CAN message
 * message  in  should be like this:
 * [  can-id ] [flags] [len] [can data ]
 */
static void asm9260_rx(struct net_device *dev)
{
	struct asm9260_priv *priv = netdev_priv(dev);
	struct net_device_stats *stats = &dev->stats;
	struct can_frame *cf;
	struct sk_buff *skb;
	uint8_t fi;
	uint8_t dreg;
	canid_t id;
	uint8_t dlc;
	int i;

	skb = dev_alloc_skb(sizeof(struct can_frame));
	if (skb == NULL)
		return;
	skb->dev = dev;
	skb->protocol = htons(ETH_P_CAN);

	fi = priv->read_reg(priv, REG_FI);
	dlc = fi & 0x0F;

	if (fi & FI_FF) {
		/* extended frame format (EFF) */
		dreg = EFF_BUF;
		id = (priv->read_reg(priv, REG_ID1) << (5 + 16))
		    | (priv->read_reg(priv, REG_ID2) << (5 + 8))
		    | (priv->read_reg(priv, REG_ID3) << 5)
		    | (priv->read_reg(priv, REG_ID4) >> 3);
		id |= CAN_EFF_FLAG;
	} else {
		/* standard frame format (SFF) */
		dreg = SFF_BUF;
		id = (priv->read_reg(priv, REG_ID1) << 3)
		    | (priv->read_reg(priv, REG_ID2) >> 5);
	}

	if (fi & FI_RTR)
		id |= CAN_RTR_FLAG;

	cf = (struct can_frame *)skb_put(skb, sizeof(struct can_frame));
	memset(cf, 0, sizeof(struct can_frame));
	cf->can_id = id;
	cf->can_dlc = dlc;
	for (i = 0; i < dlc; i++)
		cf->data[i] = priv->read_reg(priv, dreg++);

	while (i < 8)
		cf->data[i++] = 0;

	/* release receive buffer */
	priv->write_reg(priv, REG_CMR, CMD_RRB);

	netif_rx(skb);
        
	stats->rx_packets++;
	stats->rx_bytes += dlc;
}
/* 
 * asm9260 can err.
 */
static int asm9260_err(struct net_device *dev, uint8_t isrc, uint8_t status)
{
	struct asm9260_priv *priv = netdev_priv(dev);
	struct net_device_stats *stats = &dev->stats;
	struct can_frame *cf;
	struct sk_buff *skb;
	enum can_state state = priv->can.state;
	uint8_t ecc, alc;

	skb = dev_alloc_skb(sizeof(struct can_frame));
	if (skb == NULL)
		return -ENOMEM;
	skb->dev = dev;
	skb->protocol = htons(ETH_P_CAN);
	cf = (struct can_frame *)skb_put(skb, sizeof(struct can_frame));
	memset(cf, 0, sizeof(struct can_frame));
	cf->can_id = CAN_ERR_FLAG;
	cf->can_dlc = CAN_ERR_DLC;

	if (isrc & IRQ_DOI) {
		/* data overrun interrupt */
		dev_dbg(dev->dev.parent, "data overrun interrupt\n");
		cf->can_id |= CAN_ERR_CRTL;
		cf->data[1] = CAN_ERR_CRTL_RX_OVERFLOW;
		stats->rx_over_errors++;
		stats->rx_errors++;
		priv->write_reg(priv, REG_CMR, CMD_CDO);	/* clear bit */
	}

	if (isrc & IRQ_EI) {
		/* error warning interrupt */
		dev_dbg(dev->dev.parent, "error warning interrupt\n");

		if (status & SR_BS) {
			state = CAN_STATE_BUS_OFF;
			cf->can_id |= CAN_ERR_BUSOFF;
			can_bus_off(dev);
		} else if (status & SR_ES) {
			state = CAN_STATE_ERROR_WARNING;
		} else
			state = CAN_STATE_ERROR_ACTIVE;
	}
	if (isrc & IRQ_BEI) {
		/* bus error interrupt */
		priv->can.can_stats.bus_error++;
		stats->rx_errors++;

		ecc = priv->read_reg(priv, REG_ECC);

		cf->can_id |= CAN_ERR_PROT | CAN_ERR_BUSERROR;

		switch (ecc & ECC_MASK) {
		case ECC_BIT:
			cf->data[2] |= CAN_ERR_PROT_BIT;
			break;
		case ECC_FORM:
			cf->data[2] |= CAN_ERR_PROT_FORM;
			break;
		case ECC_STUFF:
			cf->data[2] |= CAN_ERR_PROT_STUFF;
			break;
		default:
			cf->data[2] |= CAN_ERR_PROT_UNSPEC;
			cf->data[3] = ecc & ECC_SEG;
			break;
		}
		/* Error occured during transmission? */
		if ((ecc & ECC_DIR) == 0)
			cf->data[2] |= CAN_ERR_PROT_TX;
	}
	if (isrc & IRQ_EPI) {
		/* error passive interrupt */
		dev_dbg(dev->dev.parent, "error passive interrupt\n");
		if (status & SR_ES)
			state = CAN_STATE_ERROR_PASSIVE;
		else
			state = CAN_STATE_ERROR_ACTIVE;
	}
	if (isrc & IRQ_ALI) {
		/* arbitration lost interrupt */
		dev_dbg(dev->dev.parent, "arbitration lost interrupt\n");
		alc = priv->read_reg(priv, REG_ALC);
		priv->can.can_stats.arbitration_lost++;
		stats->tx_errors++;
		cf->can_id |= CAN_ERR_LOSTARB;
		cf->data[0] = alc & 0x1f;
	}

	if (state != priv->can.state && (state == CAN_STATE_ERROR_WARNING ||
					 state == CAN_STATE_ERROR_PASSIVE)) {
		uint8_t rxerr = priv->read_reg(priv, REG_RXERR);
		uint8_t txerr = priv->read_reg(priv, REG_TXERR);
		cf->can_id |= CAN_ERR_CRTL;
		if (state == CAN_STATE_ERROR_WARNING) {
			priv->can.can_stats.error_warning++;
			cf->data[1] = (txerr > rxerr) ?
				CAN_ERR_CRTL_TX_WARNING :
				CAN_ERR_CRTL_RX_WARNING;
		} else {
			priv->can.can_stats.error_passive++;
			cf->data[1] = (txerr > rxerr) ?
				CAN_ERR_CRTL_TX_PASSIVE :
				CAN_ERR_CRTL_RX_PASSIVE;
		}
	}

	priv->can.state = state;

	netif_rx(skb);

	stats->rx_packets++;
	stats->rx_bytes += cf->can_dlc;

	return 0;
}
/* 
 * ASM9260 CAN interrupt.
 */
irqreturn_t asm9260_interrupt(int irq, void *dev_id)
{
	struct net_device *dev = (struct net_device *)dev_id;
	struct asm9260_priv *priv = netdev_priv(dev);
	struct net_device_stats *stats = &dev->stats;
	uint8_t isrc, status;
	int n = 0;

        isrc = priv->read_reg(priv, REG_IR);
          priv->write_reg(priv, REG_IR,0);
	/* Shared interrupts and IRQ off? */
	if (priv->read_reg(priv, REG_IER) == IRQ_OFF)
		return IRQ_NONE;

	if (priv->pre_irq)
		priv->pre_irq(priv);

	while ((isrc) && (n < ASM9260_MAX_IRQ)) {
		n++;
		status = priv->read_reg(priv, REG_SR);

		if (isrc & IRQ_WUI)
			dev_warn(dev->dev.parent, "wakeup interrupt\n");

		if (isrc & IRQ_TI) {
			/* transmission complete interrupt */
			stats->tx_bytes += priv->read_reg(priv, REG_FI) & 0xf;
			stats->tx_packets++;
			can_get_echo_skb(dev, 0);
			netif_wake_queue(dev);
		}
		if (isrc & IRQ_RI) {
			/* receive interrupt */
			while (status & SR_RBS) {
				asm9260_rx(dev);
				status = priv->read_reg(priv, REG_SR);
			}
		}
		if (isrc & (IRQ_DOI | IRQ_EI | IRQ_BEI | IRQ_EPI | IRQ_ALI)) {
			/* error interrupt */
			if (asm9260_err(dev, isrc, status))
				break;
		}
	}

	if (priv->post_irq)
		priv->post_irq(priv);

	if (n >= ASM9260_MAX_IRQ)
		dev_dbg(dev->dev.parent, "%d messages handled in ISR", n);

	return (n) ? IRQ_HANDLED : IRQ_NONE;
}
EXPORT_SYMBOL_GPL(asm9260_interrupt);

/* 
 * OPEN   ASM9260 CAN DEV.
 */
int asm9260_open(struct net_device *dev)
{
	struct asm9260_priv *priv = netdev_priv(dev);
	int err;

	/* set chip into reset mode */
	set_reset_mode(dev);

	/* common open */
	err = open_candev(dev);
	if (err)
		return err;

	/* register interrupt handler, if not done by the device driver */
	if (!(priv->flags & ASM9260_CUSTOM_IRQ_HANDLER)) {
		err = request_irq(dev->irq, &asm9260_interrupt, priv->irq_flags,
				  dev->name, (void *)dev);
		if (err) {
			close_candev(dev);
			return -EAGAIN;
		}
	}

	/* init and start chi */
	asm9260_start(dev);
	priv->open_time = jiffies;

	netif_start_queue(dev);
	return 0;
}
EXPORT_SYMBOL_GPL(asm9260_open);


/* 
 * close asm9260 can dev.
 */
int asm9260_close(struct net_device *dev)
{
	struct asm9260_priv *priv = netdev_priv(dev);

	netif_stop_queue(dev);
	set_reset_mode(dev);

	if (!(priv->flags & ASM9260_CUSTOM_IRQ_HANDLER))
		free_irq(dev->irq, (void *)dev);

	close_candev(dev);

	priv->open_time = 0;

	return 0;
}
EXPORT_SYMBOL_GPL(asm9260_close);

/* 
 * This function mainly alloc can dev.
 */
struct net_device *alloc_asm9260dev(int sizeof_priv)
{
	struct net_device *dev;
	struct asm9260_priv *priv;

	dev = alloc_candev(sizeof(struct asm9260_priv) + sizeof_priv,1);
	if (!dev)
		return NULL;

	
	priv = netdev_priv(dev);

	priv->dev = dev;
	priv->can.bittiming_const = &asm9260_bittiming_const;
	priv->can.do_set_bittiming = asm9260_set_bittiming;
	priv->can.do_set_mode = asm9260_set_mode;

	spin_lock_init(&priv->cmdreg_lock);

	if (sizeof_priv)
		priv->priv = (void *)priv + sizeof(struct asm9260_priv);

	return dev;
}
EXPORT_SYMBOL_GPL(alloc_asm9260dev);

void free_asm9260dev(struct net_device *dev)
{
	free_candev(dev);
}
EXPORT_SYMBOL_GPL(free_asm9260dev);

static const struct net_device_ops asm9260_netdev_ops = {
       .ndo_open               = asm9260_open,
       .ndo_stop               = asm9260_close,
       .ndo_start_xmit         = asm9260_start_xmit,
};

/* 
 * This function mainly register can dev.
 */
int register_asm9260dev(struct net_device *dev)
{
	if (!asm9260_probe_chip(dev))
		return -ENODEV;

	dev->flags |= IFF_ECHO;	/* we support local echo */
	dev->netdev_ops = &asm9260_netdev_ops;
	set_reset_mode(dev);
	chipset_init(dev);

	return register_candev(dev);
}
EXPORT_SYMBOL_GPL(register_asm9260dev);

/* 
 * asm9260 can mode unregister.
 */
void unregister_asm9260dev(struct net_device *dev)
{
	set_reset_mode(dev);
	unregister_candev(dev);
}
EXPORT_SYMBOL_GPL(unregister_asm9260dev);

/* 
 * asm9260 can mode init.
 */
static __init int asm9260_init(void)
{
	printk(KERN_INFO "%s CAN netdevice driver\n", DRV_NAME);

	return 0;
}

module_init(asm9260_init);

/* 
 * asm9260 can mode exit.
 */
static __exit void asm9260_exit(void)
{
	printk(KERN_INFO "%s: driver removed\n", DRV_NAME);
}

module_exit(asm9260_exit);
