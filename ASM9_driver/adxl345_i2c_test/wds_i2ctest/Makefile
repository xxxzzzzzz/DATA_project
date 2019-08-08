KERN_DIR =/home/xiang/ASM9260T/ASM9260T-linux2.6.32/linux-2.6.32.27_9260kernel

all:
	make -C $(KERN_DIR) M=`pwd` modules 

clean:
	make -C $(KERN_DIR) M=`pwd` modules clean
	rm -rf modules.order

obj-m	+= at24cxx_dev.o
obj-m	+= at24cxx_drv.o
#obj-m	+= i2c_bus_s3c2440.o
