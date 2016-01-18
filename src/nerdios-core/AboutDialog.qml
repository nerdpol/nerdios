import QtQuick 2.0
import QtQuick.Layouts 1.1
import QtQuick 2.3
import QtQuick.Controls 1.2

ApplicationWindow {
    height: 300
    width: 500

    RowLayout {
        id: rowLayout1
        anchors.fill: parent
        Image {
            id: logo
            width: parent.width / 3
            height: parent.height
            fillMode: Image.PreserveAspectFit
            source: "images/XMPP_logo.svg"
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

                Text {
                    id: text2
                    x: 0
                    y: 40
                    text: qsTr("https://github.com/nerdpol/nerdios")
                    font.pixelSize: 12
                }
            }
        }
    }



}

