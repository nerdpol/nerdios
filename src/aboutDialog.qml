import QtQuick 2.0
import QtQuick.Layouts 1.1

Item {
    height: 300
    width: 500

    RowLayout {
        id: rowLayout1
        anchors.fill: parent
        Item {
            id: logo
            width: parent.width / 3
            height: parent.height
        }
        Item {
            id: description
            width: (parent.width / 3) * 2
            height: parent.height

            Text {
                id: text1
                x: 35
                y: 45
                width: 120
                height: 30
                text: "Nerdios 0.1"
                font.bold: true
                font.pixelSize: 22
            }
        }
    }



}

