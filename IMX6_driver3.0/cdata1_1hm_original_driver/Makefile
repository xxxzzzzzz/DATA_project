obj-m +=1hm_orinagalmd.o
1hm_orinagalmd-objs=1HM_HX711X4.o IIC_analog/iic_analog.o IIC_analog/iic_analog2.o IIC_analog/iic_analog3.o  IIC_analog/iic_analog4.o
KDIR:=/home/xiang/IMX6/Cdata3.0/fsl-linux
EXTRA_CFLAGS += -I/home/xiang/IMX6/Cdata3.0/IMX6_driver/cdata1_1hm_original_driver/IIC_analog


PWD ?= $(shell pwd)

all:
	make -C $(KDIR) M=$(PWD) modules

clean:
	rm -rf *.o *.ko *.order *.symvers
