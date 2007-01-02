#
# 02-path.sh
#
# This script is normally loaded through .profile. It's split off separately
# so that it can be edited using the 'vp' alias, and re entered into the
# environment using the 'sp' alias.
#

PATH='/usr/local/bin'
PATH=${PATH}:"${HOME}/Utils"
PATH=${PATH}:'/bin'
PATH=${PATH}:'/usr/bin'
PATH=${PATH}:'.'
PATH=${PATH}:'/c/WINDOWS'
PATH=${PATH}:'/c/WINDOWS/system32'
PATH=${PATH}:'/c/WINDOWS/system32/Wbem'   # Needed for netsh

export PATH

