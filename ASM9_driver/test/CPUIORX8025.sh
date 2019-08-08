#!/bin/sh

insmod RX8025device.ko
insmod RX8025driver.ko
insmod hw_lowPowerCPUIO_ctldevice.ko
insmod hw_lowPowerCPUIO_ctldriver.ko
