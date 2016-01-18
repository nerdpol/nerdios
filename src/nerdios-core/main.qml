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

    function onConnectetd() {
        statusAvailable.enabled = true
        statusAway.enabled = true
        statusChat.enabled = true
        statusDND.enabled = true
        statusXA.enabled = true
        statusOffline.enabled = true
        contactsAddContact.enabled = true
        fileLogin.enabled = false
    }

    function onDisconnectetd() {
        statusAvailable.enabled = false
        statusAway.enabled = false
        statusChat.enabled = false
        statusDND.enabled = false
        statusXA.enabled = false
        statusOffline.enabled = false
        contactsAddContact.enabled = false
        fileLogin.enabled = true
    }

    function truncateString(msg, length) {
        // don't accept negative lengths
        if (length < 0) {
            return msg;
        }
        if (msg.length <= length) {
            return msg;
        }
        return (msg.substring(0, length - 3) + "...");
    }

    function hasFocus(recipient) {
        console.log("hasFocus:", recipient);
        if (recipient === "") {
            return;
        }

        // split jid and resource, and ignore the latter
        var jid = recipient.split("/")[0];

        var elem = messageStack.currentItem;
        console.log("hasFocus::elem.recipient:", elem.recipient);
        return (elem.recipient === jid);
    }

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
                contactInfoText.text = jid;
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
                    id: fileLogin
                    text: "Login"
                    onTriggered: loginDialog.open()
                }

                MenuItem {
                    text: "About"
                    onTriggered: {
                        var component = Qt.createComponent("AboutDialog.qml")
                        var window    = component.createObject(root)
                        window.show()
                            }
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
                    id: statusAvailable
                    text: "Available"
                    enabled: false
                    onTriggered: {
                        nerdioscore.setStatus("available")
                    }
                }
                MenuItem {
                    id: statusChat
                    text: "Chat"
                    enabled: false
                    onTriggered: {
                        nerdioscore.setStatus("chat")
                    }
                }
                MenuItem {
                    id: statusAway
                    text: "Away"
                    enabled: false
                    onTriggered: {
                        nerdioscore.setStatus("away")
                    }
                }
                MenuItem {
                    id: statusXA
                    text: "XA"
                    enabled: false
                    onTriggered: {
                        nerdioscore.setStatus("xa")
                    }
                }
                MenuItem {
                    id: statusDND
                    text: "Do Not Disturb"
                    enabled: false
                    onTriggered: {
                        nerdioscore.setStatus("do not disturb")
                    }
                }
                MenuItem {
                    id: statusOffline
                    text: "Offline"
                    enabled: false
                    onTriggered: {
                        nerdioscore.disconnect()
                    }
                }
            }

            Menu {
                title: "Contacts"
                MenuItem {
                    id: contactsAddContact
                    text: "Add contact"
                    enabled: false
                    onTriggered: addContact
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
                z: -1

                model: ContactList {
                    xmppClient: nerdioscore.xmppClient
                }

                delegate: RosterContactDelegate {
                    jid: model.jid
                    type: model.type
                    availableType: model.available_type
                    history: truncateString(model.last_message, 20)
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
            onConnectetd();
        }
        onDisconnected: {
            actionButton.text = "Connect"
            onDisconnectetd();
        }
    }
}
