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
        property string jid: "test@example.io"
        property string password: "password"
    }

    menuBar: MenuBar {
            Menu {
                title: "File"
                MenuItem { text: "Open..." }
                MenuItem { text: "Close" }
            }

            Menu {
                title: "Edit"
                MenuItem { text: "Cut" }
                MenuItem { text: "Copy" }
                MenuItem { text: "Paste" }
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

            Rectangle {
                id: roster
                Layout.fillWidth: true
                Layout.fillHeight: true
                Text {
                    text: "roster"
                }
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

            Rectangle {
                id: messageBox
                Layout.fillWidth: true
                Layout.fillHeight: true
                Text {
                    text: "messages"
                }
            }

            TextField {
                id: messageField
                Layout.fillWidth: true
                anchors.bottom: parent.bottom
                Keys.onReturnPressed: {
                    messageField.text = ""
                }
            }
        }
    }
    /*Connections {
        target: nerdioscore.xmppClient
        onConnected: {
            clientInfo.actionButton.text = "Disconnect"
        }
    }*/
}
