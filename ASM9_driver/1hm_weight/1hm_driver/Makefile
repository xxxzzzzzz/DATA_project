obj-m +=1hm_driver.o
1hm_driver-objs=1hm_hx711_driver.o 1hm_iic_analog/hx711_iic.o
KERN_DIR:=/home/xiang/ASM9260T/ASM9260T-linux/linux-2.6.32.27_9260kernel
EXTRA_CFLAGS += -I/home/xiang/ASM9260T/ASM9260T_driver/1hm_weight/1hm_driver/1hm_iic_analog
PWD ?= $(shell pwd)
all:
	make -C $(KERN_DIR) M=$(PWD) modules 

clean:
	rm -rf *.o *.ko *.symvers *.order *.mod.c
