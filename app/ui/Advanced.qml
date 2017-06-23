import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3

Item {
    Flickable {
        anchors.fill: parent
        contentHeight: advancedBox.height
        contentWidth: advancedBox.width

        ScrollBar.vertical: ScrollBar {
            hoverEnabled: true
        }

        ScrollBar.horizontal: ScrollBar {
            hoverEnabled: true
        }

        Column {
            id: advancedBox

            GroupBox {
                title: qsTr("Screen Capture Quality")

                Column {
                    RadioButton {
                        enabled: false
                        text: qsTr("Always use PNG (no lossy compression)")
                        checked: information.settingIsCompressionPng()
                        onClicked: information.settingSetCompressionPng(checked)
                    }

                    RadioButton {
                        enabled: false
                        text: qsTr("Smart (use JPG unless PNG is smaller)")
                        checked: information.settingIsCompressionSmaller()
                        onClicked: information.settingSetCompressionSmaller(checked)
                    }

                    Row {
                        Label {
                            text: qsTr("Quality: ")
                            anchors.verticalCenter: parent.verticalCenter
                        }

                        Slider {
                            enabled: false
                            from: 0
                            to: 100
                            stepSize: 1
                            value: information.getQuality()
                            onValueChanged: information.setQuality(value)
                        }
                    }
                }
            }

            GroupBox {
                title: qsTr("Fullscreen Capture")

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
