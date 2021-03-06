import QtQuick 2.7
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

Item {
    function setAccountPage(loggedIn) {
        isLoggedIn.visible = loggedIn
        notLoggedIn.visible = !loggedIn

        if (loggedIn) {
            // clear on successful login if the user was not logged in to begin with
            email.text = ""
            password.text = ""
            loggedInEmail.text = information.getEmail()
            apiKey.text = information.getApiKey()
            if (information.isPremium())
                accountType.text = qsTr("Premium")
            else
                accountType.text = qsTr("Free")
            expiryDate.text = information.getPremiumExpire()
            diskUsage.text = information.getDiskUsage()
        } else {
            loggedInEmail.text = ""
            apiKey.text = ""
            accountType.text = ""
            expiryDate.text = ""
            diskUsage.text = ""
        }
    }

    function updateAccountInformation() {
        if (authentication.isLoggedIn())
            authentication.update()
    }

    Component.onCompleted: setAccountPage(authentication.isLoggedIn())

    Connections {
        target: authentication

        onAuthMessage: {
            authMessage.text = text
            loginIndicator.running = false
        }

        onAuthChange: {
            setAccountPage(loggedIn)
            loginIndicator.running = false
            systemTray.updateHistory()
        }
    }

    ScrollView {
        anchors.fill: parent
        contentHeight: accountBox.height
        contentWidth: accountBox.width

        GroupBox {
            id: accountBox
            title: qsTr("Account Details")
            width: root.width

            Column {
                Column {
                    spacing: 10
                    id: notLoggedIn

                    Label {
                        wrapMode: Text.WrapAtWordBoundaryOrAnywhere
                        width: parent.width
                        text: qsTr("You must login to use Puush. If you don't already have an account, you can register for an account below.")
                    }

                    GridLayout {
                        columns: 2

                        Label {
                            text: "Login"
                        }

                        TextField {
                            id: email
                        }

                        Label {
                            text: "Password"
                        }

                        TextField {
                            id: password
                            echoMode: TextInput.Password
                        }
                    }

                    Label {
                        text: "<a href='" + information.settingGetPuushUrl() + "reset_password'>" + qsTr("Forgot password?") + "</a>"
                        onLinkActivated: Qt.openUrlExternally(information.settingGetPuushUrl() + "reset_password")

                        MouseArea {
                            anchors.fill: parent
                            acceptedButtons: Qt.NoButton // we don't want to eat clicks on the Text
                            cursorShape: parent.hoveredLink ? Qt.PointingHandCursor : Qt.ArrowCursor
                        }
                    }

                    Row {
                        Button {
                            text: qsTr("Submit")
                            anchors.verticalCenter: parent.verticalCenter
                            onClicked: {
                                loginIndicator.running = true
                                authentication.submitLogin(email.text, password.text)
                            }
                        }

                        BusyIndicator {
                            id: loginIndicator
                            running: false
                        }
                    }

                    Label {
                        text: "<a href='" + information.settingGetPuushUrl() + "register'>" + qsTr("Register...") + "</a>"
                        onLinkActivated: Qt.openUrlExternally(information.settingGetPuushUrl() + "register")

                        MouseArea {
                            anchors.fill: parent
                            acceptedButtons: Qt.NoButton // we don't want to eat clicks on the Text
                            cursorShape: parent.hoveredLink ? Qt.PointingHandCursor : Qt.ArrowCursor
                        }
                    }

                    Label {
                        id: authMessage
                    }
                }

                GridLayout {
                    columns: 2
                    id: isLoggedIn

                    Label {
                        text: qsTr("Logged in as: ")
                    }

                    Label {
                        id: loggedInEmail
                    }

                    Label {
                        text: qsTr("API key: ")
                    }

                    Label {
                        id: apiKey
                    }

                    Label {
                        text: qsTr("Account type: ")
                    }

                    Label {
                        id: accountType
                    }

                    Label {
                        text: qsTr("Expiry date: ")
                    }

                    Label {
                        id: expiryDate
                    }

                    Label {
                        text: qsTr("Disk usage: ")
                    }

                    Label {
                        id: diskUsage
                    }

                    Button {
                        text: qsTr("My Account")
                        onClicked: systemTray.openAccount()
                    }

                    Button {
                        text: qsTr("Logout")
                        onClicked: authentication.logout()
                    }
                }
            }
        }
    }
}
