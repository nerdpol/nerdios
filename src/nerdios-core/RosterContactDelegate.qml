import QtQuick 2.0
import QtQuick.Layouts 1.1

Component {
    id: rosterContactDelegate

    Rectangle {
        property string rectColor: "blue"
        property string jid: modelData
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
                        return "blue"
                    }
                }
            }

            ColumnLayout {
                Text {
                    text: modelData
                    font.bold: true
                    anchors.top: parent.top
                }
                Text {
                    text: "last chat histo..."
                }
            }

        }
    }
}
