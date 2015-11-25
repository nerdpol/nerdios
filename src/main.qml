import QtQuick 2.5
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.1


ApplicationWindow {
    id: root
    visible: true
    width: 490
    height: 980

    SplitView {
        anchors.fill: parent
        orientation: Qt.Horizontal

        Rectangle {
            width: 200
            Layout.minimumWidth: 200
            Layout.maximumWidth: 400
            color: "lightblue"
            Text {
                text: "Contacts"
                anchors.centerIn: parent
            }
        }
        Rectangle {
            id: centerItem
            Layout.minimumWidth: 400
            Layout.fillWidth: true
            color: "lightgray"
            Text {
                text: "Message"
                anchors.centerIn: parent
            }
        }
    }
}
