From Qt's maintenance tools, install the Qt Installer Framework.  
From its bin folder, copy `binarycreator.exe` and `installerbase.exe` to this folder.

Create a release executable of puush-qt.  
Create the folder `puush-qt/packages/puushqt/data`.  
Copy the executable and dll to `puush-qt/packages/puushqt/data`.

Run `windeployqt.exe` with --qmldir option set and pointing to the executable you copied.

Double check the running puush-qt from within the data directory works (and double check the qml loads when checking the puush-qt's settings window).

Update the version and release information in the config and meta folders.

Run `build.bat` to create the `puush-qt-installer.exe`.