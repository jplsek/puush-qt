puush-qt
========
A GUI frontend for puush on Linux. This will create a system tray icon and contains similar options to puush from Windows (not all features are, nor probably be implemented).

... This is also my first "real" qt project, so keep that in mind.

![Screenshot](screenshot.png)

## Installing
Arch Linux users can install this from the AUR with `puush-qt`.  
At the moment other users can compile this manually.

## Dependencies
`curl scrot`  
`qt4-base` or `qt5-base`

## Compiling
* Open the .pro file with qt-creator, else you can compile via command line:

```
git clone https://github.com/jplsek/puush-qt  
cd puush-qt  
mkdir build && cd build  
qmake ../src/puush-qt.pro  
make  
sudo make install # install globally, this does not create a .desktop file yet, so it can only be launched via command line and auto start files
```

## Other Information
There are a few quirks with this application.  
1. You must manually select a window after clicking the "screenshot active window" button.  
2. The active window screenshot may include extra "padding" and contain the backgroud, etc. This is probably from the window shadow or from other things.  
3. The cursor may not change when clicking "select area screenshot", just  click and drag, and it should work.

## License
BSD Clause 3
