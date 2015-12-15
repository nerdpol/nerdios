import QtQuick 2.5
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.1

import nerdioscore 1.0


ApplicationWindow {
    id: root
    visible: true
    width: 490
    height: 980

    NerdiosCore {
        id: nerdioscore
        jid: "test@example.io"
        password: "password"

        onMessageReceived: {
            messageBox.text += (message.from + ": " + message.body + "\n")
        }
    }

    menuBar: MenuBar {
            Menu {
                title: "File"
                MenuItem {
                    text: "About"
                }
                MenuItem {
                    text: "Preferences"
                }
                MenuItem {
                    text: "Quit"
                    onTriggered: Qt.quit()
                }
            }

            Menu {
                title: "Status"
                MenuItem { text: "Available" }
                MenuItem { text: "Away" }
                MenuItem { text: "Do Not Disturb" }
                MenuItem { text: "Offline" }
            }

            Menu {
                title: "Contacts"
                MenuItem { text: "Add contact" }
            }
        }

    SplitView {
        anchors.fill: parent
        orientation: Qt.Horizontal

        ColumnLayout {
            id: left
            width: parent.width / 6
            Layout.minimumWidth: parent.width / 6
            Layout.maximumWidth: parent.width / 3

            TextField {
                id: searchField
                Layout.fillWidth: true
            }

            ListView {
                id: roster
                Layout.fillWidth: true
                Layout.fillHeight: true

                model: nerdioscore.roster
                delegate: RosterContactDelegate { }
            }

            Rectangle {
                id: clientInfo
                Layout.fillWidth: true
                height: 100
                ColumnLayout {
                    Text {
                        text: nerdioscore.jid
                    }
                    Text {
                        text: nerdioscore.xmppClient.state
                    }
                    Button {
                        id: actionButton
                        text: "Connect"
                        onClicked: nerdioscore.connect()
                    }
                }
            }
        }
        ColumnLayout {
            id: right
            Rectangle {
                id: contactInfo
                Layout.fillWidth: true
                height: 50
                Text {
                    text: "contact info"
                }
            }

            TextArea {
                id: messageBox
                Layout.fillWidth: true
                Layout.fillHeight: true
            }

            TextField {
                id: messageField
                Layout.fillWidth: true
                anchors.bottom: parent.bottom
                Keys.onReturnPressed: {
                    nerdioscore.sendMessage("", messageField.text)
                    messageBox.text += (nerdioscore.jid + ": " + messageField.text + "\n")
                    messageField.text = ""
                }
            }
        }
    }

    Connections {
        target: nerdioscore.xmppClient
        onConnected: {
            actionButton.text = "Disconnect"
        }
        onDisconnected: {
            actionButton.text = "Connect"
        }
    }
}
