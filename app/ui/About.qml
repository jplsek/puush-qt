import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3

Item {
    Image {
        id: logo
        x: 0
        width: 100
        height: 100
        source: "qrc:/images/puush-qt.png"
        anchors.top: parent.top
    }

    Label {
        id: title
        padding: 50
        text: qsTr("puush-qt")
        horizontalAlignment: Text.AlignHCenter
        font.bold: true
        font.pixelSize: 48
        anchors.top: parent.top
        anchors.left: logo.right
        width: parent.width - logo.width
    }

    Label {
        id: version
        text: qsTr("Version ") + information.getVersion()
        horizontalAlignment: Text.AlignHCenter
        font.pixelSize: 12
        anchors.top: title.bottom
        anchors.left: logo.right
        width: parent.width - logo.width
    }

    Button {
        anchors.bottom: parent.bottom
        text: "About Qt"
        onClicked: information.getQtAbout()
    }
}
