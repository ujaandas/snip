import QtQuick
import QtQuick.Window

Window {
    width: 480
    height: 240
    visible: true
    title: "Hello World"

    Rectangle {
        anchors.fill: parent
        color: "#111111"

        Text {
            anchors.centerIn: parent
            text: "Hello World"
            color: "#f5f5f5"
            font.pixelSize: 32
            font.bold: true
        }
    }
}