#!/bin/bash
# check to see if the system uses /usr/local/lib for shared libraries
# Arch and Red Hat does not use it by default.
grep -r /usr/local/lib /etc/ld.so.conf.d/ > /dev/null

if [ $? != 0 ]; then
    echo "/usr/local/lib" > /etc/ld.so.conf.d/puush-qt.conf
    ldconfig
fi
