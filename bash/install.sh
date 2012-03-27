#!/bin/bash
#
# Installs the files in this directory into the users home directory
#

#set -x

script=$0
scriptdir=$(dirname ${script})

verbose=false
dry_run=false

DIR=${scriptdir}/

osName=
case "$(uname)" in
    CYGWIN*)
        osName=cygwin
        ;;

    Linux)
        osName=linux
        ;;
esac

function InstallDir
{
    local srcDir=$1
    local dstDir=$2
    local srcFile
    local dstFile
    local srcBase

    $verbose && echo "InstallDir '${srcDir}' '${dstDir}' ..."

    for srcFile in ${srcDir}/*
    do  
        $verbose && echo "Processing '${srcFile}' ..."

        srcBase=$(basename ${srcFile})
        
        if [ "${srcBase}" = "install.sh" ]
        then
            # That's this file - skip it
            
            ${verbose} && echo "-- Skipping install.sh"
            continue
        fi

        # Change files which look like dot-foo to be .foo
        
        if [ "${srcBase:0:4}" = "dot-" ]
        then
            dstFile=${dstDir}/.${srcBase:4}
            ${verbose} && echo "  Renaming '${srcFile}' to '${dstFile}'"
        else
            dstFile=${dstDir}/${srcBase}
        fi

        # Install the file

        if [ -d ${srcFile} ]
        then
            #
            # We're installing a subdirectory - recurse
            #
            
            mkdir -p ${dstFile}
            InstallDir ${srcFile} ${dstFile}
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
                        dstFile=${dstFile%-cygwin.sh}.sh
                    fi
                    ;;

                *-linux.sh)
                    if [ "${osName}" = "linux" ]
                    then
                        installThisFile=1
                        dstFile=${dstFile%-linux.sh}.sh
                    fi
                    ;;

                *)
                    installThisFile=1
                    ;;
            esac

            if [ "${installThisFile}" = "1" ]
            then
                echo "Installing '${srcFile}' to '${dstFile}' ..."
                ! ${dry_run} && cp -p ${srcFile} ${dstFile}
            else
                echo "Skipping   '${srcFile}' ..."
            fi
        fi
    done

}

InstallDir ${scriptdir} ~

#
# Now add a call to ~/.startup into .bashrc
#

if ! fgrep .startup ~/.bashrc
then
	echo "" >> ~/.bashrc
	echo ". ~/.startup" >> ~/.bashrc
fi
