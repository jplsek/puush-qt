import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3

Item {
    Connections {
        target: keyBindings

        onSignalUploadFile: systemTray.uploadFile();
        onSignalCaptureDesktop: systemTray.fullScreenScreenshot();
        onSignalCaptureWindow: systemTray.activeWindowScreenshotTimed();
        onSignalCaptureArea: systemTray.selectAreaScreenshot();
        onSignalUploadClipboard: systemTray.uploadClipboard();
        onSignalTogglePuush: systemTray.togglePuush();
    }

    function handleOnPressed(item, event, fnGetKey) {
        if (event.key === Qt.Key_Escape) {
            item.text = fnGetKey()
            item.focus = false
        } else {
            item.text = keyBindings.getKeySequence(event.modifiers, event.key)
        }
    }

    function handleOnReleased(item, event, fnSetKey) {
        if (keyBindings.isProperBinding(event.modifiers, event.key)) {
            fnSetKey(item.text)
            item.focus = false
        } else {
            item.text = qsTr("Bad combination. Try again...")
        }
    }

    Flickable {
        anchors.fill: parent
        contentHeight: keyBindingsBox.height
        contentWidth: keyBindingsBox.width

        ScrollBar.vertical: ScrollBar {
            hoverEnabled: true
        }

        ScrollBar.horizontal: ScrollBar {
            hoverEnabled: true
        }

        GroupBox {
            id: keyBindingsBox
            title: qsTr("Key Bindings")

            Column {
                spacing: 10

                GridLayout {
                    columns: 3

                    Label {
                        text: qsTr("Capture desktop: ")
                    }

                    Button {
                        id: captureDesktop
                        text: keyBindings.getCaptureDesktop()
                        onClicked: text = qsTr("Enter combination...")
                        Keys.onPressed: handleOnPressed(this, event, keyBindings.getCaptureDesktop)
                        Keys.onReleased: handleOnReleased(this, event, keyBindings.setCaptureDesktop)
                        onFocusChanged: if (!focus) text = keyBindings.getCaptureDesktop()
                    }

                    Button {
                        text: qsTr("Reset")
                        onClicked: captureDesktop.text = keyBindings.resetCaptureDesktop()
                    }

                    Label {
                        text: qsTr("Capture current window: ")
                    }

                    Button {
                        id: captureWindow
                        text: keyBindings.getCaptureWindow()
                        onClicked: text = qsTr("Enter combination...")
                        Keys.onPressed: handleOnPressed(this, event, keyBindings.getCaptureWindow)
                        Keys.onReleased: handleOnReleased(this, event, keyBindings.setCaptureWindow)
                        onFocusChanged: if (!focus) text = keyBindings.getCaptureWindow()
                    }

                    Button {
                        text: qsTr("Reset")
                        onClicked: captureWindow.text = keyBindings.resetCaptureWindow()
                    }

                    Label {
                        text: qsTr("Capture area: ")
                    }

                    Button {
                        id: captureArea
                        text: keyBindings.getCaptureArea()
                        onClicked: text = qsTr("Enter combination...")
                        Keys.onPressed: handleOnPressed(this, event, keyBindings.getArea)
                        Keys.onReleased: handleOnReleased(this, event, keyBindings.setArea)
                        onFocusChanged: if (!focus) text = keyBindings.getCaptureArea()
                    }

                    Button {
                        text: qsTr("Reset")
                        onClicked: captureArea.text = keyBindings.resetCaptureArea()
                    }

                    Label {
                        text: qsTr("Upload file: ")
                    }

                    Button {
                        id: uploadFile
                        text: keyBindings.getUploadFile()
                        onClicked: text = qsTr("Enter combination...")
                        Keys.onPressed: handleOnPressed(this, event, keyBindings.getUploadFile)
                        Keys.onReleased: handleOnReleased(this, event, keyBindings.setUploadFile)
                        onFocusChanged: if (!focus) text = keyBindings.getUploadFile()
                    }

                    Button {
                        text: qsTr("Reset")
                        onClicked: uploadFile.text = keyBindings.resetUploadFile()
                    }

                    Label {
                        text: qsTr("Upload clipboard: ")
                    }

                    Button {
                        id: uploadClipboard
                        text: keyBindings.getUploadClipboard()
                        onClicked: text = qsTr("Enter combination...")
                        Keys.onPressed: handleOnPressed(this, event, keyBindings.getUploadClipboard)
                        Keys.onReleased: handleOnReleased(this, event, keyBindings.setUploadClipboard)
                        onFocusChanged: if (!focus) text = keyBindings.getUploadClipboard()
                    }

                    Button {
                        text: qsTr("Reset")
                        onClicked: uploadClipboard.text = keyBindings.resetUploadClipboard()
                    }

                    Label {
                        text: qsTr("Toggle puush-qt functionality: ")
                    }

                    Button {
                        id: togglePuush
                        text: keyBindings.getTogglePuush()
                        onClicked: text = qsTr("Enter combination...")
                        Keys.onPressed: handleOnPressed(this, event, keyBindings.getTogglePuush)
                        Keys.onReleased: handleOnReleased(this, event, keyBindings.setTogglePuush)
                        onFocusChanged: if (!focus) text = keyBindings.getTogglePuush()
                    }

                    Button {
                        text: qsTr("Reset")
                        onClicked: togglePuush.text = keyBindings.resetTogglePuush()
                    }
                }

                Text {
                    width: parent.width
                    wrapMode: Text.WrapAtWordBoundaryOrAnywhere
                    text: qsTr("If the key combination does not get activated (or does not get accepted when changing the combination), it may mean that another application is using that key combination.")
                }
            }
        }
    }
}
