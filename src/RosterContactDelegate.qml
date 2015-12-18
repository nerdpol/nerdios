import QtQuick 2.0
import QtQuick.Layouts 1.1

Component {
    id: rosterContactDelegate

    Rectangle {
        height: 50
        width: parent.width
        MouseArea {
            anchors.fill: parent
            onClicked: {
                console.log(modelData)
                focusChat(modelData, "")
            }
        }

        RowLayout {
            Rectangle {
                height: 50
                width: 50
                color: "blue"
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
