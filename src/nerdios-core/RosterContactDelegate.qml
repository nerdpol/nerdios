import QtQuick 2.0
import QtQuick.Layouts 1.1

Rectangle {
    id: root
    property string jid: ""
    property string type: ""
    property string availableType: ""
    property string rectColor: "blue"
    height: 50
    width: parent.width
    MouseArea {
        anchors.fill: parent
        onClicked: {
            console.log(jid)
            focusChat(jid, "")
            rectColor = "blue" // reset color
        }
    }

    RowLayout {
        Rectangle {
            height: 50
            width: 50
            color: {
                console.log("updating roster")
                if (jid === nerdioscore.lastUser) {
                    nerdioscore.lastUser = ""
                    return "red"
                } else {
                    if (type === "Available" && availableType === "Away") {
                        return "orange"
                    }
                    return "blue"
                }
            }
        }

        ColumnLayout {
            Text {
                text: jid
                font.bold: true
                anchors.top: parent.top
            }
            Text {
                text: "last chat histo..."
            }
        }

    }
}
