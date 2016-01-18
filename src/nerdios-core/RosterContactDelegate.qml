import QtQuick 2.0
import QtQuick.Layouts 1.1

Rectangle {
    id: root
    property string jid: ""
    property string type: ""
    property string availableType: ""
    property string history: ""

    height: 50
    width: parent.width

    MouseArea {
        anchors.fill: parent
        onClicked: {
            console.log(jid)
            var elem = getChatWindow(jid, true);
            if (elem === undefined) {
                createChatWindow(jid);
                getChatWindow(jid, true);
            }
            rect.color = "blue"; // reset color
            history = "";
        }
    }

    RowLayout {
        Rectangle {
            id: rect
            height: 50
            width: 50
            color: {
                console.log("updating roster");
                var lastUser = nerdioscore.lastUser.split("/")[0];
                if (jid === lastUser) {
                    nerdioscore.lastUser = "";
                    if (!hasFocus(lastUser)) {
                        return "red";
                    }
                } else {
                    if (type === "Available" && availableType === "Away") {
                        return "orange";
                    }
                }
                return "blue";
            }
        }

        ColumnLayout {
            Text {
                text: jid
                font.bold: true
                anchors.top: parent.top
            }
            Text {
                text: history
            }
        }

    }
}
