./wifi_signalCheck.sh > wifihotspot
awk '{print $1 " " $2 " " $3 " " $4 " " $5 " " $7 " " $8 " " $9 " " $10 " " $11 " " $12 " " $13 " " $14 " " $15 " " $16 " " $6}' wifihotspot > hotspot
