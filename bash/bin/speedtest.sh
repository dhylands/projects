#!/bin/bash
#
# This script acts as a frontend for speedtest-cli, available from:
#
# git clone https://github.com/sivel/speedtest-cli.git
#
# speedtest-cli --list | grep Burnaby
#
# shows:
#
# 1375) In2net Network Inc. (Burnaby, BC, Canada) [244.33 km]
#
# To install the script, add the following line to /etc/cron.d/rsnapshot
#
#   30 1  	* * *		root	/home/dhylands/projects/bash/bin/speedtest.sh

#set -x

if [ "$(id -u)" == "0" ]; then
    LOG=/var/log/speedtest-last-run.log
    REAL_LOG=/var/log/speedtest.log
else
    LOG=speedtest-last-run.log
    REAL_LOG=speedtest.log
fi

HOSTNAME=$(hostname)
SPEEDTEST_SERVER=1375
SPEEDTEST_DIR=/home/dhylands/software/speedtest-cli

echo "$(date): ==============================================" > ${LOG}
echo "$(date): Starting speedtest of ${SPEEDTEST_SERVER} from ${HOSTNAME}" >> ${LOG}

CMD="${SPEEDTEST_DIR}/speedtest-cli --simple --server ${SPEEDTEST_SERVER}"
echo "$(date): ${CMD}" >> ${LOG}
${CMD} | while read line; do
    echo "$(date): ${line}" >> ${LOG}
done
echo "$(date): Done" >> ${LOG}

cat ${LOG}
cat ${LOG} >> ${REAL_LOG}

