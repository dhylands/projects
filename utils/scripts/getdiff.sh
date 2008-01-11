#!/bin/sh

# getdiff.sh:
#
# A simple shell script to find the difference between the given
# file, say, "foo/bar" and "foo/bar.orig". The output is a 
# "diff -u2N" context difference preceded by "Index: " and "===="
# lines somewhat similar to that output by CVS. 
#
# This output can be treated as a patch against the original 
# file (without the ".orig" suffix) to convert it to the given 
# file like this:
#
#     patch -p0 < output-from-this-script
#
# Author: Ranjit Mathew (rmathew@hotmail.com)
#

if test -z $1; then
  echo
  echo ERROR: Required parameter missing!
  echo
  echo USAGE: getdiff.sh \<foo\>
  echo Finds the difference between the files \<foo.orig\> and \<foo\>
  exit 1
fi

if test ! -f $1; then
  echo ERROR: File $1 does not exist
  exit 1;
fi

if test ! -f $1.orig; then
  echo ERROR: File $1.orig does not exist
  exit 1;
fi

echo Index: $1
echo ===================================================================
NAME4SED=`echo $1 |sed 's/\\//\\\\\\//g'`
diff -uN -U 2 $1.orig $1 |sed s/$NAME4SED\\.orig/$NAME4SED/g

# END (getdiff.sh)
