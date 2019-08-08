obj-m +=ds1302_driver.o
ds1302_driver-objs=ds1302rtc.o ds1302_rw.o
KDIR:=/home/xiang/ASM9260T/ASM9260T-linux2.6.32/linux-2.6.32.27_9260kernel
EXTRA_CFLAGS += -I/home/xiang/ASM9260T/ASM9260T_driver/DS1302_RTC/ds1302_driver


PWD ?= $(shell pwd)

all:
	make -C $(KDIR) M=$(PWD) modules

clean:
	rm -rf *.o *.ko *.order *.symvers *.mod.c
