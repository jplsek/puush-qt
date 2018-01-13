puush-qt
========
A GUI frontend for puush on Linux. This will create a system tray icon and contains similar options to puush from Windows (not all features are, nor probably will be implemented).

![Screenshot](screenshot.png)

## Installing
Arch Linux users can install this from the AUR with `puush-qt`.  

Windows user can grab the latest installer [here](https://github.com/jplsek/puush-qt/releases).

For now, if you aren't on Arch Linux, see [Compiling](#compiling) below.

## Dependencies
Arch: `scrot qt5-{base,declarative,quickcontrols,quickcontrols2}`  
Ubuntu: `scrot qt5-default qml-module-qtquick-{controls,controls2,templates2,dialogs}`

## Compiling
### Extra Build Dependencies
Ubuntu 17.04 or newer: `qtbase5-private-dev libxcb-keysyms1-dev qtdeclarative5-dev`  
For other Ubuntu versions, you will need qt5.7 or newer. PPA's are available [here](https://launchpad.net/~beineri/+ppa-packages).  
This is an example for 16.04 with qt5.7:

```
sudo add-apt-repository ppa:beineri/opt-qt571-xenial
sudo apt-get install qt57-meta-minimal qt57quickcontrols2 qt57quickcontrols
qtchooser -install qt5.7 /opt/qt57/bin/qmake
export QT_SELECT=qt5.7
```

### Building

* Open puush-qt.pro file with qt-creator, else you can compile via command line:

```
git clone --recursive https://github.com/jplsek/puush-qt  
cd puush-qt  
mkdir build && cd build  
qmake ../puush-qt.pro  
make  
sudo make install
```

## Other Information
There are a few quirks with this application.  
1. You must manually select a window after clicking the "screenshot active window" button.  
2. For GNOME users, the active window screenshot may include extra "padding" and contain the backgroud.  
3. The cursor may not change when clicking "select area screenshot", just  click and drag, and it should work.

Passwords are not stored. They are forgotten after getting the api key from Puush.

* Mac is not supported only because I don't own a Mac. Everything should compile, but OS specific screenshot stuff may not be implemented (I'm not sure how the "select area" will work, and "select window" is definitely not implemented. Fullscreen screenshots should work fine.). This is because qt does not support certain desktop features like detecting the current active window. See src/screenshot.cpp. Contributions are welcome. Eventually, I may borrow a laptop from a friend, or some other means, to get feature and build parity implemented.

## License
BSD Clause 3
