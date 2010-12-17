#
# 02-path.sh
#
# This script is normally loaded through .profile. It's split off separately
# so that it can be edited using the 'vp' alias, and re entered into the
# environment using the 'sp' alias.
#

AddPath()
{
    if [ -d "$1" ]
    then
        if [ -z "${PATH}" ]
        then
            PATH="$1"
        else
            PATH=${PATH}:"$1"
        fi
    fi
}

PATH=''
AddPath "${HOME}/bin"
AddPath "${HOME}/utils"
AddPath '/usr/local/bin'
AddPath '/usr/local/bin'
AddPath '/usr/local/sbin'
AddPath '/bin'
AddPath '/sbin'
AddPath '/usr/bin'
AddPath '/usr/sbin'
AddPath '/usr/X11R6/bin'
AddPath '.'
AddPath '/c/WinAVR/bin'
AddPath '/c/Program Files/CodeSourcery/Sourcery G++/bin'
AddPath '/c/WINDOWS'
AddPath '/c/WINDOWS/system32'

#needed for netsh
AddPath '/c/WINDOWS/system32/Wbem'

export PATH

