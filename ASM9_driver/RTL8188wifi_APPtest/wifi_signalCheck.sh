#!/bin/sh

iwlist wlan0 scanning | egrep -e "Cell|Quality" >wifi_sg
cat wifi_sg |grep "Cell" > wifi_SSIDname
cat wifi_sg |grep "Quality" > wifi_signal

num=$(cat wifi_SSIDname | wc -l)
space="    "
h="\n"
for i in `seq 1 $num` 
do
	a=$(sed -n "${i}p" wifi_SSIDname)
	b=$(sed -n "${i}p" wifi_signal)
	echo -e $a$space$b$h
done
