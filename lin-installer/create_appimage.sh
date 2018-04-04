#!/bin/bash
# create_deb.sh must be ran first.
cd ../build
source /opt/qt57/bin/qt57-env.sh
dpkg -x puush-qt*.deb root

wget -c "https://github.com/probonopd/linuxdeployqt/releases/download/continuous/linuxdeployqt-continuous-x86_64.AppImage"
chmod +x linuxdeployqt*.AppImage
unset QTDIR; unset QT_PLUGIN_PATH; unset LD_LIBRARY_PATH

mkdir -p root/usr/qml/QtQuick
cp -r /opt/qt57/qml/QtQuick/Dialogs root/usr/qml/QtQuick
mkdir -p root/usr/plugins
cp -r /opt/qt57/plugins/xcbglintegrations root/usr/plugins

./linuxdeployqt*.AppImage root/usr/share/applications/puush-qt.desktop \
  -qmldir=/opt/qt57/qml \
  -bundle-non-qt-libs \
  -appimage
