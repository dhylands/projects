#!/bin/bash
#
# This script actss as a frontend for rsnapshot. It checks to see if 
# my laptop is connected to my network at home, and if it is, then
# it fires up the backup.
#
# Note: 

LOG=/var/log/backup.log
BACKUP_DIR=/backup-dave/dave-ubuntu

HOSTNAME=$(hostname)

echo "$(date): HOSTNAME = ${HOSTNAME}" >> ${LOG}

CHECK_WIRELESS=0

case ${HOSTNAME} in

    # These machines are always attached to the network, so don't
    # need the wireless check

    dave-ubuntu)
        CHECK_WIRELESS=0
        ;;

    dave-laptop)
        CHECK_WIRELESS=1
        ;;

    *)
        echo "$(date): Unrecognized hostname '${HOSTNAME}'" >> ${LOG}
        exit 0
        ;;

esac

if [ ${CHECK_WIRELESS} = 1 ];
then
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
    
    
    SSID=$(/sbin/iwconfig eth1 | grep ESSID | sed -e 's/^.*ESSID://' -e 's/ .*//' -e 's/"//g')
    
    echo "====================================================" >> ${LOG}
    if [ "${SSID}" != "Blue-Heron" ]
    then
        echo "$(date): Not on home network" >> ${LOG}
        echo "  SSID = ${SSID}" >> ${LOG}
        exit 0
    fi
fi

#
# Since we may not be connected to the network all of the time, we run this
# script often, but only do the backups when the oldest backup of a given
# level is more than it's period old. So when weekly.0 is more than a week
# old then we do a weekly backup.
#

daySecs=$(( 24 * 60 * 60 ))
weekSecs=$(( $daySecs * 7 ))
monthSecs=$(( $daySecs * 365 / 12 ))

currTime=$(date '+%s')

#
# Always do a daily backup
#

echo "$(date): Started daily rsnapshot" >> ${LOG}
nice /usr/bin/rsnapshot -v daily >> ${LOG} 2>&1
echo "$(date): Finished daily rsnapshot" >> ${LOG}

weekStamp=$(find ${BACKUP_DIR}/ -maxdepth 1 -name weekly.0 -printf '%T@\n')
weekStamp=${weekStamp%.*}

if [ $(( $weekStamp + $weekSecs + $weekSecs )) -lt $currTime ]
then
    # weekly.0 is more than two weeks old. Do a weekly backup.
    # We need to use two weeks since daily.6 will become weekly.0
    # and daily.6 is already a week old

    echo "$(date): Started weekly rsnapshot" >> ${LOG}
    nice /usr/bin/rsnapshot -v weekly >> ${LOG} 2>&1
    echo "$(date): Finished weekly rsnapshot" >> ${LOG}
fi

monthStamp=$(find ${BACKUP_DIR}/ -maxdepth 1 -name monthly.0 -printf '%T@\n')

if [ $(( $monthStamp + $monthSecs + $monthSecs )) -lt $currTime ]
then
    # monthly.0 is more than two months old. Do a monthly backup.
    # weekly.4 will become monthly.0

    echo "$(date): Started monthly rsnapshot" >> ${LOG}
    nice /usr/bin/rsnapshot -v monthly >> ${LOG} 2>&1
    echo "$(date): Finished monthly rsnapshot" >> ${LOG}
fi


