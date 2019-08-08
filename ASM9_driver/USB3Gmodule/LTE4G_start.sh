#!/bin/sh
sleep 10
./scsi4G
var=$(ls /dev/ | grep "USB0")
while [ "ttyUSB0" != "$var"  ]
do
	sleep 1
	var=$(ls /dev/ | grep "USB0")
done
mount -t tmpfs tmfps /var/run
mount -t tmpfs tmfps /var/lock
cd /etc/ppp/peers/
pppd call wcdma-dailer &

ppp=$(ifconfig | grep "ppp0" | awk '{print $1}')

while [ "ppp0" != "$ppp"  ]
do
	sleep 1
	ppp=$(ifconfig | grep "ppp0" | awk '{print $1}')
done
cp /etc/ppp/resolv.conf /etc/
cd /

