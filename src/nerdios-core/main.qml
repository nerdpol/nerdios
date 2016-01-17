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

    function createChatWindow(recipient) {
        console.log("createChatWindow:", recipient);
        if (recipient === "") {
            return;
        }

        // split jid and resource, and ignore the latter
        var jid = recipient.split("/")[0];

        var elem = messageStack.find(function(item) {
            if (item !== null && item.recipient === jid) {
                return true;
            }
            return false;
        }, true);

        console.log("createChatWindow::elem:", elem);
        // create new chat window
        if (elem === null) {
            var newObject = Qt.createQmlObject('import QtQuick.Controls 1.4; TextArea {readOnly: true; property string recipient}',
                root, "dynamicSnippet1");
            messageStack.push({item: newObject, properties: {recipient: jid}});
        }
    }

    function getChatWindow(recipient, focus) {
        console.log("getChatWindow:", recipient, focus);
        if (recipient === "") {
            return;
        }

        // split jid and resource, and ignore the latter
        var jid = recipient.split("/")[0];

        var elem = messageStack.find(function(item) {
            if (item !== null && item.recipient === jid) {
                return true;
            }
            return false;
        }, true);

        console.log("getChatWindow::elem:", elem);
        if (elem !== null) {
            // automatically focus if only one chat window exist
            if (messageStack.depth === 1 || focus === true) {
                messageStack.pop(elem);
                messageField.readOnly = false;
                contactInfoText.text = recipient;
            }
            return elem;
        }
    }

    function addMessage(recipient, message) {
        if (recipient === "" || message === "") {
            return;
        }

        // split jid and resource, and ignore the latter
        var jid = recipient.split("/")[0];

        var elem = getChatWindow(recipient, false);
        console.log("addMessage::elem:", elem);
        if (elem === undefined) {
            createChatWindow(recipient);
            elem = getChatWindow(recipient, false);
        }

        elem.text += jid + ": " + message + "\n"
    }

    Dialog {
        id: addContactDialog
        title: "Add a contact"
        standardButtons: StandardButton.Ok | StandardButton.Cancel
        GridLayout {
            id: addConteactGrid
            columns: 2
            rows: 1
            Label {
                text: "JID: "
            }
            TextField {
                id: jidField
            }
        }
        onAccepted: {
            console.log(jidField.text)
            nerdioscore.addContact(jidField.text)
        }
    }

    Dialog {
        id: loginDialog
        title: "Login"
        standardButtons: StandardButton.Ok | StandardButton.Cancel
        GridLayout {
            id: loginDialogGrid
            columns: 2
            rows: 2
            Label {
                text: "Username: "
            }
            TextField {
                id: loginName
            }
            Label {
                text: "Password: "
            }
            TextField {
                id: loginPassword
                echoMode: TextInput.Password
            }
        }

        onAccepted: {
            nerdioscore.jid = loginName.text
            nerdioscore.password = loginPassword.text
            nerdioscore.connect()
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
                    text: "Login"
                    onTriggered: loginDialog.open()
                }

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
                    }
                }
                MenuItem {
                    text: "Away"
                    onTriggered: {
                        nerdioscore.setStatus("away")
                    }
                }
                MenuItem {
                    text: "Do Not Disturb"
                    onTriggered: {
                        nerdioscore.setStatus("do not disturb")
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
                Layout.margins: 5
            }

            ListView {
                id: roster
                Layout.fillWidth: true
                Layout.fillHeight: true
                Layout.margins: 5

                model: ContactList {
                    xmppClient: nerdioscore.xmppClient
                }

                delegate: RosterContactDelegate {
                    jid: model.jid
                    type: model.type
                    availableType: model.available_type
                }
                spacing: 5
            }

            Rectangle {
                id: clientInfo
                Layout.fillWidth: true
                height: 80
                ColumnLayout {
                    anchors.left: parent.left
                    anchors.leftMargin: 10
                    anchors.verticalCenter: parent.verticalCenter
                    Text {
                        text: nerdioscore.jid
                    }
                    RowLayout {
                        id: statusInfo
                        Text {
                            id: clientPresence
                            text: nerdioscore.state
                        }
                        Text {
                            text: "-"
                        }

                        Text {
                            id: clientStatus
                            text: nerdioscore.status
                        }
                    }
                    Button {
                        id: actionButton
                        text: "Connect"
                        onClicked: {
                            if (text == "Connect") {
                                if (nerdioscore.jid && nerdioscore.password) {
                                    nerdioscore.connect();
                                } else {
                                    loginDialog.open();
                                    loginName.focus = true;
                                }
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
            Layout.bottomMargin: 5
            Layout.margins: 5
            Rectangle {
                id: contactInfo
                Layout.fillWidth: true
                height: 50
                color: "transparent"

                Text {
                    id: contactInfoText
                    Layout.leftMargin: 10
                    text: "Select a contact"
                    anchors.verticalCenter: parent.verticalCenter
                    verticalAlignment: Text.AlignTop
                    font.pointSize: 20
                    font.bold: true
                }
            }

            StackView {
                id: messageStack
                Layout.fillWidth: true
                Layout.fillHeight: true
                delegate: StackViewDelegate {
                        pushTransition: PropertyAnimation {}
                        popTransition: PropertyAnimation {}
                }
            }
            TextField {
                id: messageField
                readOnly: true
                Layout.fillWidth: true
                Layout.margins: 5
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
        target: nerdioscore
        onMessageReceived: {
            addMessage(message.from, message.body)
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
