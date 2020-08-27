import QtQuick 2.7
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

Item {
    ScrollView {
        anchors.fill: parent
        contentHeight: advancedBox.height
        contentWidth: advancedBox.width

        Column {
            id: advancedBox

            GroupBox {
                title: qsTr("Fullscreen Capture")
                width: root.width

                Column {
                    RadioButton {
                        text: qsTr("Capture all screens")
                        checked: information.settingIsFullscreenAllDesktops()
                        onClicked: information.settingSetFullscreenAllDesktops(checked)
                    }

                    RadioButton {
                        text: qsTr("Capture screen containing mouse cursor")
                        checked: information.settingIsFullscreenCurrentDesktop()
                        onClicked: information.settingSetFullscreenCurrentDesktop(checked)
                    }

                    RadioButton {
                        text: qsTr("Always capture primary screen")
                        checked: information.settingIsFullscreenPrimaryDesktop()
                        onClicked: information.settingSetFullscreenPrimaryDesktop(checked)
                    }
                }
            }
            GroupBox {
                title: "API Location"
                width: root.width

                GridLayout {
                    columns: 2

                    Label {
                        text: qsTr("Puush URL: ")
                    }

                    TextField {
                        text: information.settingGetPuushUrl()
                        onTextChanged: {
                            information.settingSetPuushUrl(text)
                            systemTray.updateHistory()
                        }
                    }

                    Label {
                        text: qsTr("Puush API URL: ")
                    }

                    TextField {
                        text: information.settingGetPuushApiUrl()
                        onTextChanged: {
                            information.settingSetPuushApiUrl(text)
                            systemTray.updateHistory()
                        }
                    }
                }
            }
        }
    }
}
