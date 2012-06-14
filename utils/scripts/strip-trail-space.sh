#!/bin/sh

for f in $@
do
    sed 's/[ ]*$//' $f > $f._strip_
    if $(cmp --quiet $f $f._strip_)
    then
        # Files are identical

        rm $f._strip_
    else
        # Some trailing spaces were removed

        mv $f._strip_ $f
        echo "Removed trailing white space from $f"
    fi
done


