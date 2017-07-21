obj-m +=mcp2515_can.o
mcp2515_can-objs=mcp2515.o mcp2515_spi_analog/mcp2515_spi.o
KDIR:=/home/xiang/IMX6/Cdata4.0/fsl-linux
EXTRA_CFLAGS += -I/home/xiang/IMX6/Cdata4.0/IMX6_driver/can/mcp2515_spi_analog

PWD ?= $(shell pwd)

all:
	make -C $(KDIR) M=$(PWD) modules

clean:
	rm -rf *.o *.ko *.symvers *.order
