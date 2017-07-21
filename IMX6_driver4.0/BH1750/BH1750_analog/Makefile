obj-m +=bh1750md.o
bh1750md-objs=bh1750m.o bh1750_rw.o 
KDIR:=/home/xiang/IMX6/fsl-linux
EXTRA_CFLAGS += -I/home/xiang/IMX6/IMX-driver/BH1750/BH1750_analog


PWD ?= $(shell pwd)

all:
	make -C $(KDIR) M=$(PWD) modules

clean:
	rm -rf *.o
