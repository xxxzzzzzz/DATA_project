obj-m +=rgb_led.o

KDIR:=/home/xiang/IMX6/Cdata3.0/fsl-linux
PWD=$(shell pwd)
all:
	make -C $(KDIR) M=$(PWD) modules
clean:
	rm -rf *.o *.ko *.order *.symvers
