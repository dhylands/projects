###########################################################################
#
# 21-merge.sh
#
# Sets up a function to run Araxis merge using cygwin style paths
#

function merge
{
    local -a fileList
    local    idx

    idx=0
    for file in "$@"
    do
      fileList[${idx}]=$(cygpath -w "${file}")
      idx=$(( ${idx} + 1 ))
    done

    cmd /c start "" 'c:\Program Files\Araxis\Araxis Merge 2001 v6.0 Professional\Merge.exe' "${fileList[@]}"
}

