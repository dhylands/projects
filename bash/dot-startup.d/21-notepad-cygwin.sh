###########################################################################
#
# 21-notepad.sh
#
# Sets up a function to run notepad using cygwin style paths
#
###########################################################################

function np
{
   local -a fileList
   local idx

   idx=0

   for file in "$@"
   do
      fileList[${idx}]=$(cygpath -w "${file}")
      idx=$(( ${idx} + 1 ))
   done
   start notepad "${fileList[@]}"
}

