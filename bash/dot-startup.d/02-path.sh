#
# 02-path.sh
#
# This script is normally loaded through .profile. It's split off separately
# so that it can be edited using the 'vp' alias, and re entered into the
# environment using the 'sp' alias.
#

PATH='/usr/local/bin'
PATH=${PATH}:"${HOME}/Utils"
PATH=${PATH}:'/c/Program Files/ImageMagick-6.2.8-Q16'
PATH=${PATH}:'/bin'
PATH=${PATH}:'/usr/bin'
PATH=${PATH}:'/usr/X11R6/bin'
PATH=${PATH}:'.'
PATH=${PATH}:'/c/WinAVR/bin'
PATH=${PATH}:'/c/Program Files/CodeSourcery/Sourcery G++/bin'
PATH=${PATH}:'/c/WINDOWS'
PATH=${PATH}:'/c/WINDOWS/system32'
PATH=${PATH}:'/c/WINDOWS/system32/Wbem'   # Needed for netsh

export PATH

