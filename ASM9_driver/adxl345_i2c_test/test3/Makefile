
KERN_DIR:=/home/xiang/ASM9260T/ASM9260T-linux2.6.32/linux-2.6.32.27_9260kernel
obj-m += i2c_adxl345_test.o  

PWD ?= $(shell pwd)
all:
	make -C $(KERN_DIR) M=$(PWD) modules 

clean:
	rm -rf *.o *.ko *.symvers *.order *.mod.c
