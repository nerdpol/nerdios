import QtQuick 2.0

Component {
    id: rosterContactDelegate

    Text {
        text: modelData
        MouseArea {
            anchors.fill: parent
            onClicked: {
                console.log(modelData)
                focusChat(modelData, "")
            }
        }
    }
}
