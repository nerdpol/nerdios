import QtQuick 2.5
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.1
import QtQuick.Dialogs 1.2

import nerdioscore 1.0


ApplicationWindow {
    id: root
    visible: true
    width: 490
    height: 980

    function focusChat(recipient, message) {
        if (recipient === null) {
            return;
        }

        var jid = recipient.split("/")[0]

        if (messageStack.currentItem !== null && messageStack.currentItem.recipient === jid) {
            if (message !== "") {
                messageStack.currentItem.text += jid + ": " + message + "\n"
            }
            return;
        }

        var elem = messageStack.find(function(item) {
            if (item !== null && item.recipient === jid) {
                return true;
            }
            return false;
        }, true)
        if (elem !== null) {
            elem = messageStack.pop(elem)
            if (message !== "") {
                elem.text += jid + ": " + message + "\n"
            }
            return;
        }

        // create new chat window
        var newObject = Qt.createQmlObject('import QtQuick.Controls 1.4; TextArea {property string recipient}',
            root, "dynamicSnippet1");
        if (message === "") {
            messageStack.push({item: newObject, properties: {recipient: jid}})
        } else {
            messageStack.push({item: newObject, properties: {recipient: jid, text: jid + ": " + message + "\n"}})
        }
    }

    NerdiosCore {
        id: nerdioscore
        jid: "test@example.io"
        password: "password"

        onMessageReceived: {
            focusChat(message.from, message.body)
        }
    }

    Dialog {
        id: addContactDialog
        title: "Add a contact"
        standardButtons: StandardButton.Ok | StandardButton.Cancel
            TextField {
                id: jidField
                text: "jid"
            }
        onAccepted: {
            console.log(jidField.text)
            nerdioscore.addContact(jidField.text)
        }
    }

    Action {
        id: addContact
        onTriggered: addContactDialog.open()
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
                MenuItem {
                    text: "Available"
                    onTriggered: {
                        nerdioscore.setStatus("available")
                        clientPresence.text = "available"
                    }
                }
                MenuItem {
                    text: "Away"
                    onTriggered: {
                        nerdioscore.setStatus("away")
                        clientPresence.text = "away"
                    }
                }
                MenuItem {
                    text: "Do Not Disturb"
                    onTriggered: {
                        nerdioscore.setStatus("do not disturb")
                        clientPresence.text = "do not disturb"
                    }
                }
                MenuItem {
                    text: "Offline"
                    onTriggered: {
                        nerdioscore.disconnect()
                    }
                }
            }

            Menu {
                title: "Contacts"
                MenuItem {
                    text: "Add contact"
                    action: addContact
                }
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
                        id: clientPresence
                        text: nerdioscore.xmppClient.state
                    }
                    Button {
                        id: actionButton
                        text: "Connect"
                        onClicked: {
                            if (text == "Connect") {
                                nerdioscore.connect();
                            } else {
                                nerdioscore.disconnect();
                            }
                        }
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

            StackView {
                id: messageStack
                Layout.fillWidth: true
                Layout.fillHeight: true
            }

            TextField {
                id: messageField
                Layout.fillWidth: true
                anchors.bottom: parent.bottom
                Keys.onReturnPressed: {
                    console.log("recipient:", messageStack.currentItem)
                    nerdioscore.sendMessage(messageStack.currentItem.recipient, messageField.text)
                    messageStack.currentItem.text += nerdioscore.jid + ": " + messageField.text + "\n"
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
