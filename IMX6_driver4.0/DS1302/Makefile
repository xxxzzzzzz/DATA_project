obj-m +=ds1302web.o
ds1302web-objs=ds1302.o ds1302_rw.o
KDIR:=/home/xiang/IMX6/Cdata4.0/fsl-linux
EXTRA_CFLAGS += -I/home/xiang/IMX6/Cdata4.0/IMX6_driver/DS1302


PWD ?= $(shell pwd)

all:
	make -C $(KDIR) M=$(PWD) modules

clean:
	rm -rf *.o *.ko *.symvers *.order
