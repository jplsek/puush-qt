#!/bin/bash
# update the icon chache if root (since arch builds the package as a normal
# user and pacman updates the cache itself)
# http://stackoverflow.com/a/33446531

if [[ $EUID == 0 ]] && [[ ! "$LD_LIBRARY_PATH:" = *libfakeroot:* ]]; then
    gtk-update-icon-cache /usr/share/icons/hicolor
fi

