
obj-m += dht11_device.o  
KERN_DIR:=/home/xiang/ASM9260T/ASM9260T-linux/linux-2.6.32.27_9260kernel

PWD ?= $(shell pwd)
all:
	make -C $(KERN_DIR) M=$(PWD) modules 

clean:
	rm -rf *.o *.ko *.symvers *.order *.mod.c
