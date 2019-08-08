obj-m +=ds2401_driver.o
ds2401_driver-objs=ds2401_driver_code.o ds2401.o
KERN_DIR:=/home/xiang/ASM9260T/ASM9260T-linux/linux-2.6.32.27_9260kernel
EXTRA_CFLAGS += -I/home/xiang/ASM9260T/ASM9260T_driver/DS2401/ds2401_driver
PWD ?= $(shell pwd)
all:
	make -C $(KERN_DIR) M=$(PWD) modules 

clean:
	rm -rf *.o *.ko *.symvers *.order *.mod.c
