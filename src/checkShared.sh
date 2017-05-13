#!/bin/bash
# check to see if the system uses /usr/local/lib for shared libraries
# when installing manually.
# Arch and Red Hat does not use it by default.
# Also check if we are running as root and not in fakeroot.
# http://stackoverflow.com/a/33446531

if [[ $EUID == 0 ]] && [[ ! "$LD_LIBRARY_PATH:" = *libfakeroot:* ]]; then
    grep -r "/usr/local/lib" /etc/ld.so.conf.d/ > /dev/null 2>&1

    if [ $? != 0 ]; then
        mkdir -p /etc/ld.so.conf.d
        echo "/usr/local/lib" > /etc/ld.so.conf.d/puush-qt.conf
        ldconfig
    fi
fi
