import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3
import QtQuick.Window 2.0

ApplicationWindow {
    id: root
    visible: false
    width: bar.implicitWidth
    height: 400
    minimumWidth: 400
    minimumHeight: 300
    title: "puush-qt"

    Connections {
        target: systemTray

        onSignalOpenSettings: {
            root.show()
            bar.currentIndex = tab
        }
    }

    TabBar {
        id: bar
        x: 0
        y: 0
        width: root.width

        TabButton {
            text: qsTr("General")
        }

        TabButton {
            text: qsTr("Key Bindings")
        }

        TabButton {
            text: qsTr("Account")
            onClicked: account.updateAccountInformation()
        }

        TabButton {
            text: qsTr("Advanced")
        }

        TabButton {
            text: qsTr("About")
        }
    }

    StackLayout {
        id: mainLayout
        width: parent.width
        height: parent.height - bar.height
        currentIndex: bar.currentIndex
        anchors.top: bar.bottom
        clip: true

        General { }

        KeyBindings { }

        Account {
            id: account
        }

        Advanced { }

        About { }
    }

    onClosing: {
        root.hide()
    }
}
