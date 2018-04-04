#!/bin/bash
# Run in the directory of this file.
sudo apt-get install -qy checkinstall
mkdir ../build
cd ../build
source /opt/qt57/bin/qt57-env.sh
qmake PREFIX=/usr ..
make
sudo checkinstall \
  --pkgname=puush-qt \
  --pkgversion="0" \
  --pkgrelease="2" \
  --backup=no \
  --fstrans=no \
  --default \
  --deldoc \
  --maintainer="jeremyplsek@gmail.com" \
  --pkglicense="bsd"
  #--requires="scrot \(\>=0.8\),libxcb-keysyms1 \(\>= 0.4\),qt57-meta-minimal \(\>= 5.7\),qt57quickcontrols2 \(\>= 5.7\),qt57quickcontrols \(\>=5.7\)"
  #--requires="scrot \(\>=0.8\),libxcb-keysyms1 \(\>= 0.4\),qt5-default \(\>=5.7\),qml-module-qtquick-controls2 \(\>= 5.7\),qml-module-qtquick-controls \(\>=5.7\)"
