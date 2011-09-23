#!/bin/bash
#
# Installs the files in this directory into the users home directory
#

#set -x

if [ "$(echo ${DIR}*)" = "${DIR}"'*' ]
then
    # We've been asked to install an empty directory - nothing to do

    echo "${DIR} is an empty directory"
    exit 0;
fi

osName=
case "$(uname)" in
    CYGWIN*)
        osName=cygwin
        ;;

    Linux)
        osName=linux
        ;;
esac

for srcFile in ${DIR}*
do
    if [ "${srcFile}" = "install.sh" ]
    then
        # That's this file - skip it

        continue
    fi

    # Change files which look like dot-foo to be .foo
    
    if [ "${srcFile:0:4}" = "dot-" ]
    then
        dstFile=~/.${srcFile:4}
    else
        dstFile=~/${srcFile}
    fi

    # Install the file

    if [ -d ${srcFile} ]
    then
        #
        # We're installing a subdirectory - recurse
        #
        
        mkdir -p ${dstFile}

        DIR="${srcFile}/" $0
    else
        #
        # We're installing an individual file - check for os specific variants
        #

        installThisFile=0
        case "${srcFile}" in

            *-cygwin.sh)
                if [ "${osName}" = "cygwin" ]
                then
                    installThisFile=1
                fi
                ;;

            *-linux.sh)
                if [ "${osName}" = "linux" ]
                then
                    installThisFile=1
                fi
                ;;

            *)
                installThisFile=1
                ;;
        esac

        if [ "${installThisFile}" = "1" ]
        then
            echo "Installing ${srcFile} to ${dstFile} ..."
            cp ${srcFile} ${dstFile}
        else
            echo "Skipping   ${srcFile} ..."
        fi
    fi
done

if ! fgrep .startup ~/.bashrc
then
	echo "" >> ~/.bashrc
	echo ". ~/.startup" >> ~/.bashrc
fi
