#!/bin/sh

# makepatch.sh: 
# A quick and dirty shell script to create a patchset from
# the changes made to an original source tree. The 
# patch is echoed to the standard output of the script.
#
# NOTE: This script uses the "getdiff.sh" script so you
# must have that script before you can run this script.
#
# Assumes that a file from the original source tree is first 
# backed up by copying it to a file with the same name but 
# with a ".orig" suffix in the same folder. For example, if 
# you wish to modify "fastjar/Makefile.in", copy it first to
# "fastjar/Makefile.in.orig" and then make your changes to
# "fastjar/Makefile.in".
#
# The patch thus created can then be applied to an unmodified
# source tree of the software by executing:
#
#    patch -p 0 -b -z .orig </path/to/patch/file.diff
#
# in the root source folder. (The -b and the -z flags to patch 
# back up the original files with a ".orig" suffix and are 
# optional.)
#
# Author: Ranjit Mathew (rmathew@hotmail.com)
#


# Check if the user has provided us with the name of the root
# folder of the source tree.
#
# If he has, check that the folder actually exists.
#
if [[ $# != 1 ]]; 
then
    echo
    echo makepatch.sh: Create a unified patch from modified source files
    echo
    echo USAGE: makepatch.sh \<source-root-folder-name\>
    echo
    exit 1
elif [[ ! -d $1 ]];
then
    echo
    echo ERROR: Source root folder \"$1\" does not exist!
    echo
    exit 2
fi

#cd $1

CHG_FILES=`find $1 -type f -name "*.orig" -print`

CHG_FILES=`echo $CHG_FILES | sed 's/^\.\///g' | sed 's/ \.\// /g'`
CHG_FILES=`echo "$CHG_FILES " | sed 's/\.orig / /g'`

for i in $CHG_FILES
do
    getdiff.sh $i
done

# END (makepatch.sh)
