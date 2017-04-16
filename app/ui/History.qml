import QtQuick 2.0
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3

Item {
    GroupBox {
        title: qsTr("History")
        anchors.fill: parent

        Text {
            text: qsTr("Not implemented...")
        }

        Button {
            text: qsTr("get history")
            onClicked: history.getHistory()
        }
    }
}
