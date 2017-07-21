obj-m +=ds1302rtc.o
ds1302rtc-objs=ds1302_rtc.o ds1302_rw.o
KDIR:=/home/xiang/IMX6/fsl-linux
EXTRA_CFLAGS += -I/home/xiang/IMX6/IMX-driver/DS1302/DS1302_RTC


PWD ?= $(shell pwd)

all:
	make -C $(KDIR) M=$(PWD) modules

clean:
	rm -rf *.o
