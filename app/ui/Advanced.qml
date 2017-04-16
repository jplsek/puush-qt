import QtQuick 2.0
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3

Item {
    GroupBox {
        id: captureQuality
        title: qsTr("Screen Capture Quality")
        width: parent.width

        Column {
            RadioButton {
                text: qsTr("Always use PNG (no lossy compression)")
                checked: information.settingIsCompressionPng()
                onClicked: information.settingSetCompressionPng(checked)
            }

            RadioButton {
                text: qsTr("Smart (use JPG unless PNG is smaller)")
                checked: information.settingIsCompressionSmaller()
                onClicked: information.settingSetCompressionSmaller(checked)
            }

            Row {
                Text {
                    text: qsTr("Quality: ")
                    anchors.verticalCenter: parent.verticalCenter
                }

                Slider {
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
        id: fullscreenCapture
        title: qsTr("Fullscreen Capture")
        width: parent.width
        anchors.top: captureQuality.bottom

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
}
