#!/bin/sh
echo 0 > /sys/Cusba_POWER/usba_POWER

echo "[info] waiting usb modem..."

PID=""
while true
do
        var=$(lsusb | grep 05c6:f000)
	if [ "" != "$var"  ]
	then
		PID=f000
		break
	fi
	
        var=$(lsusb | grep 05c6:1000)
	if [ "" != "$var"  ]
	then
		PID=1000
		break
	fi
	
        var=$(lsusb | grep 05c6:6000)
	if [ "" != "$var"  ]
	then
		PID=6000
		break
	fi

        sleep 1
done

if [ ${PID} == "6000" ]
then
	echo "[info] usb modem already switched to 05c6:${PID}"
	killall pppd 1&>/dev/null 2&>dev/null
else
	echo "[info] usb modem 05c6:${PID} plugged success"
	./usb_modeswitch -v 05c6 -p ${PID} -M "5553424308306384c000000080000671030000000000000000000000000000"
	
	sleep 10
	
	mount -t tmpfs tmfps /var/run
	mount -t tmpfs tmfps /var/lock
fi
                
echo "[info] start connect"
                
cd /etc/ppp/peers/
pppd call wcdma-dailer &

ppp=$(ifconfig | grep "ppp0" | awk '{print $1}')

while [ "ppp0" != "$ppp"  ]
do
        sleep 1
        ppp=$(ifconfig | grep "ppp0" | awk '{print $1}')
done
cp /etc/ppp/resolv.conf /etc/
                
echo "[info] connected to LTE success"
                
./Lserver_time_test
                
