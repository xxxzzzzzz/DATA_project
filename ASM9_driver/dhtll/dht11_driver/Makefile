obj-m +=dht11_driver.o
dht11_driver-objs=dht11_driver_code.o dht11.o
KERN_DIR:=/home/xiang/ASM9260T/ASM9260T-linux/linux-2.6.32.27_9260kernel
EXTRA_CFLAGS += -I/home/xiang/ASM9260T/ASM9260T_driver/dhtll/dht11_driver
PWD ?= $(shell pwd)
all:
	make -C $(KERN_DIR) M=$(PWD) modules 

clean:
	rm -rf *.o *.ko *.symvers *.order *.mod.c
