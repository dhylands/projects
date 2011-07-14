#!/bin/bash

## ArchiveCD.sh Script whipped up for LinuxJournal.com Blog
## Post on Archiving CD's to ISO Images.  Written by Jayson
## Broughton.  Script updates may be found at the following
## website: www.jaysonbroughton.com
##
## blocksize and blockcount variables taken from Steve Litt's
## script on Troubleshooters.com article 'Coasterless CD
## burning.
## URL: http://www.troubleshooters.com/linux/coasterless.htm
##
## Last Updated: 05/15/2011

## Check HAL for CDrom and grab UDI
UDI=`hal-find-by-capability --capability storage.cdrom`

## Run UDI against block device
device=`hal-get-property --udi $UDI --key block.device`

## Get Block size of CD
blocksize=`isoinfo -d -i $device | grep "^Logical block size is:" | cut -d " " -f 5`
if test "$blocksize" = ""; then
        echo catdevice FATAL ERROR: Blank blocksize >&2
        exit
fi

## Get Block count of CD
blockcount=`isoinfo -d -i $device | grep "^Volume size is:" | cut -d " " -f 4`
if test "$blockcount" = ""; then
        echo catdevice FATAL ERROR: Blank blockcount >&2
        exit
fi

usage()
{
cat <<EOF

usage: $0 options
-h      Show this message
-d      Report the Location of your Device
-m      Check your MD5Hash of CD against Image (Run AFTER making Image)
-l      Location and name of ISO Image (/path/to/image.iso)
-r      Rip CD to ISO image
I'm Lazy, I didn't build much error checking into this script So alas, here's how to run it. Anything else might break the script.

Example 1: Report location of drive
archiveCD.sh -d

Example 2: Rip a CD to ISO
archiveCD.sh -l /path/to/isoimage.iso -r

Example 3: Check MD5Hash (Run AFTER ripping CD to ISO)
archiveCD.sh -l /path/to/isoimage.iso -m


EOF
}



while getopts "hdml:r" OPTION; do
  case $OPTION in
    h)
      usage
      exit 1
       ;;
    d)
      echo "Your CDrom is located on: $device" >&2
      ;;
    m)
      echo "Checking MD5Sum of CD and New ISO Image"
      md5cd=`dd if=$device bs=$blocksize count=$blockcount | md5sum` >&2
      md5iso=`cat $LFLAG | md5sum` >&2
      echo "CD MD5 is:" $md5cd
      echo "ISO MD5 is:" $md5iso
      ;;
    l)
     LFLAG="$OPTARG"
      ;;
    r)
     dd if=$device bs=$blocksize count=$blockcount of=$LFLAG
     echo "Archiving Complete.  ISO Image located at:"$LFLAG
      ;;
  esac
done

