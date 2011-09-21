#!/bin/sh
#
# Script which checks to see if we're at home
#
# Check to see if we're on my wireless LAN
#
# For reference, the output of iwconfig looks like this when associated:
#
# eth1      IEEE 802.11b/g  ESSID:"Seeker-WLAN"  Nickname:"Broadcom 4306"
#           Mode:Managed  Frequency=2.437 GHz  Access Point: 00:00:00:00:00:00   
#           Bit Rate=24 Mb/s   Tx-Power=15 dBm   
#           RTS thr:off   Fragment thr:off
#           Link Quality=60/100  Signal level=-62 dBm  Noise level=-72 dBm
#           Rx invalid nwid:0  Rx invalid crypt:30678  Rx invalid frag:0
#           Tx excessive retries:0  Invalid misc:0   Missed beacon:0

SSID=$(/sbin/iwconfig 2>&1 | grep ESSID | sed -e 's/^.*ESSID://' -e 's/ .*//' -e 's/"//g')

case "${SSID}" in

    Blue-Heron*)
        echo "$(date): SSID: ${SSID}: Home network detected"
        exit 0
        ;;

    *)
        echo "$(date): SSID ${SSID}: Not on home network"
        exit 1
        ;;
esac

