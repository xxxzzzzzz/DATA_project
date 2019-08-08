obj-m = RX8025driver.o
RX8025driver-objs= RX8025SAdriver.o  RX8025SA_rw.o 
KERN_DIR:=/home/xiang/ASM9260T/ASM9260T-linux2.6.32/linux-2.6.32.27_9260kernel
EXTRA_CFLAGS += -I/home/xiang/ASM9260T/ASM9260T_driver/RX8025/RX8025SA/RX8025SAdriver

PWD ?= $(shell pwd)
all:
	make -C $(KERN_DIR) M=$(PWD) modules 

clean:
	rm -rf *.o *.ko *.symvers *.order *.mod.c
