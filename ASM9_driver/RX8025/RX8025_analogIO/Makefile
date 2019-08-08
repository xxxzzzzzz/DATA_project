obj-m += rx8025analogIO.o  
rx8025analogIO-objs=rtx8025rtc.o RX8025_rw.o
KERN_DIR:=/home/xiang/ASM9260T/ASM9260T-linux2.6.32/linux-2.6.32.27_9260kernel
EXTRA_CFLAGS += -I/home/xiang/ASM9260T/ASM9260T_driver/RX8025/RX8025_analogIO/
PWD ?= $(shell pwd)
all:
	make -C $(KERN_DIR) M=$(PWD) modules 

clean:
	rm -rf *.o *.ko *.symvers *.order *.mod.c
