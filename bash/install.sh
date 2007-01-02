#!/bin/sh
#
# Installs the files in this directory into the users home directory
#

#set -x

for file in *
do
    if [ "${file}" = "install.sh" ]
    then
        # That's this file - skip it

        continue
    fi
    
    if [ "${file:0:4}" = "dot-" ]
    then
        install_file=~/.${file:4}
    else
        install_file=~/${file}
    fi

    echo "Installing ${file} to ${install_file} ..."

    if [ -d ${file} ]
    then
        mkdir -p ${install_file}
        cp ${file}/* ${install_file}
    else
        cp ${file} ${install_file}
    fi
done
