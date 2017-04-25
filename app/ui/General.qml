import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3
import QtQuick.Dialogs 1.2

Item {
    FileDialog {
        id: fileDialog
        title: "Please choose a location"
        folder: shortcuts.pictures
        selectFolder: true
        // remove the "file://" from the url
        onAccepted: localFileLocation.text = fileUrl.toString().replace("file://", "")
    }

    Flickable {
        anchors.fill: parent
        contentHeight: generalBox.height
        contentWidth: generalBox.width

        ScrollBar.vertical: ScrollBar {
            hoverEnabled: true
        }

        ScrollBar.horizontal: ScrollBar {
            hoverEnabled: true
        }

        Column {
            id: generalBox

            GroupBox {
                title: qsTr("On Successful Puush")

                Column {
                    CheckBox {
                        text: qsTr("Play a sound")
                        checked: information.settingIsOnPuushSound()
                        onClicked: information.settingSetOnPuushSound(checked)
                        enabled: false
                    }

                    CheckBox {
                        id: localSaveCheckBox
                        text: qsTr("Enable local save")
                        checked: information.settingIsLocalSaveEnabled()
                        onClicked: information.settingSetLocalSaveEnabled(checked)
                    }

                    RowLayout {
                        TextField {
                            id: localFileLocation
                            text: information.settingGetLocalSavePath()
                            onTextChanged: information.settingSetLocalSavePath(text)
                            enabled: localSaveCheckBox.checked
                        }

                        Button {
                            text: qsTr("Browse...")
                            onClicked: fileDialog.open()
                            enabled: localSaveCheckBox.checked
                        }
                    }

                    CheckBox {
                        text: qsTr("Copy link to clipboard")
                        checked: information.settingIsOnPuushCopyLinkToClipboard()
                        onClicked: information.settingSetOnPuushCopyLinkToClipboard(checked)
                    }

                    CheckBox {
                        text: qsTr("Open link in browser")
                        checked: information.settingIsOnPuushOpenLinkInBrowser()
                        onClicked: information.settingSetOnPuushOpenLinkInBrowser(checked)
                    }
                }
            }

            GroupBox {
                title: qsTr("Tray Icon Behavior on Double Click")

                Column {
                    RadioButton {
                        text: qsTr("Show settings window")
                        checked: information.settingIsTrayClickActionOpenSettings()
                        onClicked: information.settingSetTrayClickActionOpenSettings(checked)
                    }

                    RadioButton {
                        text: qsTr("Open screenshot directory")
                        checked: information.settingIsTrayClickActionOpenUploads()
                        onClicked: information.settingSetTrayClickActionOpenUploads(checked)
                    }

                    RadioButton {
                        text: qsTr("Upload file")
                        checked: information.settingIsTrayClickActionUploadFile()
                        onClicked: information.settingSetTrayClickActionUploadFile(checked)
                    }

                    RadioButton {
                        text: qsTr("Upload clipboard")
                        checked: information.settingIsTrayClickActionUploadClipboard()
                        onClicked: information.settingSetTrayClickActionUploadClipboard(checked)
                    }

                    RadioButton {
                        text: qsTr("Capture desktop")
                        checked: information.settingIsTrayClickActionCaptureDesktop()
                        onClicked: information.settingSetTrayClickActionCaptureDesktop(checked)
                    }

                    RadioButton {
                        text: qsTr("Capture area")
                        checked: information.settingIsTrayClickActionCaptureArea()
                        onClicked: information.settingSetTrayClickActionCaptureArea(checked)
                    }

                    RadioButton {
                        text: qsTr("Capture current window")
                        checked: information.settingIsTrayClickActionCaptureWindow()
                        onClicked: information.settingSetTrayClickActionCaptureWindow(checked)
                    }

                    RadioButton {
                        text: qsTr("Open account")
                        checked: information.settingIsTrayClickActionOpenAccount()
                        onClicked: information.settingSetTrayClickActionOpenAccount(checked)
                    }
                }
            }
        }
    }
}