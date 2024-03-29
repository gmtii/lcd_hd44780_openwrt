#! /bin/sh -
#Script para initegración de OpenWrt con pantalla HD44780 y PIC
#Versión de display HD77480 20x4 líneas.
#0x15,0x16,0x17,0x18 son los caracteres para selecionar cada una de las ç
#lineas del cld.
#Este script se ejecutrada en /etc/rc.local por ejemplo.
#
# https://github.com/gmtii/lcd_hd44780_openwrt

stty 115200 < /dev/ttyS0

echo GMT-1 > /etc/TZ

trap 'exit 1' SIGINT

# activamos la pantalla para mostrar caracteres desde el router
echo -ne '\x1A' > /dev/ttyS0

# borramos pantalla
echo -ne '\x1B' > /dev/ttyS0

while true        # loop forever

do

# obtenemos información del MPD

       mpd=$(echo "currentsong" | nc localhost 6600 | grep -e "^Name: "|cut -c 7-30)
       echo -ne '\x15'$mpd > /dev/ttyS0
       mpd1=$(echo "currentsong" | nc localhost 6600 | grep -e "^Title: "|cut -c 8-27)
       mpd2=$(echo "currentsong" | nc localhost 6600 | grep -e "^Title: "|cut -c 28-47)
       mpd3=$(echo "currentsong" | nc localhost 6600 | grep -e "^Title: "|cut -c 47-66)
       
       echo -ne '\x16'$mpd1 > /dev/ttyS0
       echo -ne '\x17'$mpd2 > /dev/ttyS0
       echo -ne '\x18'$mpd3 > /dev/ttyS0
       sleep 5

# borramos pantalla

	echo -ne '\x1B' > /dev/ttyS0

# creamos algunas variables con información del router

        hora=$(date | cut -c 12-19)
	memfree=$(free | grep Mem| cut -c 41-45)
	uptime=$(uptime | cut -c 14-19)
	load=$(uptime | cut -c 36-40)
	rate=$(iwconfig wlan0 | grep -e Rate| cut -c 20-28)
	tx=$( ifconfig wlan0| grep -e MiB| cut -c 30-37)
	
        echo -ne '\x15'"Hora: "$hora > /dev/ttyS0
	echo -ne '\x16'"Memoria: "$memfree > /dev/ttyS0
	echo -ne '\x17'"Wifi: "$rate" " > /dev/ttyS0
	echo -ne '\x18'"Recibido: "$tx> /dev/ttyS0
	sleep 5

# borramos pantalla

	echo -ne '\x1B' > /dev/ttyS0

done									

done

