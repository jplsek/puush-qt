From Qt's maintenance tools, install the Qt Installer Framework.  
From its bin folder, copy `binarycreator.exe` and `installerbase.exe` to this folder.

Create a release executlable of puush-qt.  
Create the folder `puush-qt/packages/puushqt/data`.  
Copy the executlable and dll to `puush-qt/packages/puushqt/data`.

I have no idea if there is an automatic way to detect qml application shared libraries, so this is how to manually set up the data directory.

Go to Qt's bin folder, and copy all the dll's to the same folder.  
From the Qt's qml folder, copy Qt, QtQuick, and QtQuick.2 to the same folder.  
Delete all .qml files (keeping the .qmlc files).  
`puush-qt.exe` should now start.
While it's running, make sure the settings window works.
Assuming it works (and while it's still running), try to delete ALL dll's (including subdirectories) in the folder EXCEPT d3d, libEGL, and libGLESv2 (graphical fallback libraries). Windows will not allow you to delete the dll's that are in use.  
Now the hard part.  
To make the installer have less bloat, look through each subdirectory, and delete folders and qml files that are not needed. To check if they are not needed, restart `puush-qt.exe` and see if the settings window still works. Check the previous installers to see what may be necessary and removed.


Finally, once the data folder looks good, update the version and release information in the config and meta folders.

Run `build.bat` to create the `puush-qt-installer.exe`.