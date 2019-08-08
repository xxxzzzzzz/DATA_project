#!/bin/sh
var1=$(lsusb | grep "Realtek" | awk '{print $7}')
var2=$(ls /dev/ | grep "sr0")
if [ "Realtek" = "$var1" ];
then
	echo "wifi insert success"
	cd /Cdatabox/test/
	./wlan_sta.sh
	cd /
elif [ "sr0" = "$var2" ];
then
	echo "4G model insert success"
	./LTE4G_start.sh
else
	echo "wifi 4G insert failed"
fi
