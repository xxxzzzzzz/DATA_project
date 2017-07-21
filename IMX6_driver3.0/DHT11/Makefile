obj-m +=temphumisencer.o
temphumisencer-objs=temphumi.o dht11.o
KDIR:=/home/xiang/IMX6/Cdata3.0/fsl-linux
EXTRA_CFLAGS += -I/home/xiang/IMX6/IMX-driver/DHT11


PWD ?= $(shell pwd)

all:
	make -C $(KDIR) M=$(PWD) modules

clean:
	rm -rf *.o *.ko *.symvers *.order
