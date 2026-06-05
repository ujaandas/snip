import QtQuick
import QtQuick.Controls

ApplicationWindow {
    visible: true
    width: 900
    height: 600

    EditorView {
        anchors.fill: parent
        controller: editor
    }
}