import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3
import QtQuick.Window 2.0

ApplicationWindow {
    id: root
    visible: true
    width: bar.implicitWidth
    height: 600
    minimumWidth: 400
    minimumHeight: 400
    title: "puush-qt"

    Connections {
        target: systemTray

        onSignalOpenSettings: {
            if(root.visibility === Window.Hidden) {
                root.show()
            } else {
                root.hide()
            }
        }
    }

    TabBar {
        id: bar
        x: 0
        y: 0
        width: root.width

        TabButton {
            id: generalTab
            text: qsTr("General")
        }

        TabButton {
            id: keybindingsTab
            text: qsTr("Key Bindings")
        }

        TabButton {
            id: accountTab
            text: qsTr("Account")
        }

        TabButton {
            id: advancedTab
            text: qsTr("Advanced")
        }

        TabButton {
            id: historyTab
            text: qsTr("History")
        }

        TabButton {
            id: aboutTab
            text: qsTr("About")
        }
    }

    StackLayout {
        id: mainLayout
        width: parent.width
        height: parent.height - bar.height
        currentIndex: bar.currentIndex
        anchors.top: bar.bottom

        General { }

        KeyBindings { }

        Account { }

        Advanced { }

        History { }

        About { }
    }

    onClosing: {
        root.hide()
    }
}
