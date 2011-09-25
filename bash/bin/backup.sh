#!/bin/bash
#
# This script actss as a frontend for rsnapshot. It checks to see if 
# my laptop is connected to my network at home, and if it is, then
# it fires up the backup.
#
# To install the script, add the following line to /etc/cron.d/rsnapshot
#
#   00 2  	* * *		root	/home/dhylands/projects/bash/backup.sh daily
#
# cron should pick up the change automatically

scriptname=$(basename $0)
scriptdir=$(dirname $0)

LOG=/var/log/backup-last-run.log
REAL_LOG=/var/log/backup.log

HOSTNAME=$(hostname)

echo "$(date): ==============================================" > ${LOG}
echo "$(date): Starting backup of HOSTNAME = ${HOSTNAME}" >> ${LOG}

BACKUP_MOUNT=/backup-dave
BACKUP_DIR=${BACKUP_MOUNT}/${HOSTNAME}

# Check to see if we're at home or not.

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
    if ! ${scriptdir}/check-wireless.sh >> ${LOG}
    then
        exit 1
    fi
fi

#
# Check to see that /backup-dave is mounted, and if not
# then mount it
#

if mount | grep ${BACKUP_MOUNT} > /dev/null
then
    echo "$(date): ${BACKUP_MOUNT} appears to be mounted." >> ${LOG}
else
    echo "$(date): ${BACKUP_MOUNT} doesn't appear to be mounted - trying to mount" >> ${LOG}

    # Volume doesn't appear to be mounted. Mount it now
    # so we don't backup locally

    if mount ${BACKUP_MOUNT} > /dev/null
    then
        echo "$(date): ${BACKUP_MOUNT} mounted successfully" >> ${LOG}
    else
        echo "$(date): Unable to mount ${BACKUP_MOUNT} - aborting backup" >> ${LOG}
        exit 1
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
monthStamp=${monthStamp%.*}

if [ $(( $monthStamp + $monthSecs + $monthSecs )) -lt $currTime ]
then
    # monthly.0 is more than two months old. Do a monthly backup.
    # weekly.4 will become monthly.0

    echo "$(date): Started monthly rsnapshot" >> ${LOG}
    nice /usr/bin/rsnapshot -v monthly >> ${LOG} 2>&1
    echo "$(date): Finished monthly rsnapshot" >> ${LOG}
fi

cat ${LOG}
cat ${LOG} >> ${REAL_LOG}

