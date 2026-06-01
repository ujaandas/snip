import QtQuick
import QtQuick.Window
import MyEditor 1.0

Window {
    width: 480
    height: 240
    visible: true
    title: "Hello World!!!"

    Rectangle {
        anchors.fill: parent
        color: "#111111"

        EditorView {
            anchors.fill: parent
        }
    }
}