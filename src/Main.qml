import QtQuick
import QtQuick.Window
import MyEditor 1.0
import MyScroll 1.0

Window {
    width: 800
    height: 600
    visible: true

    EditorScroll {
        anchors.fill: parent

        EditorView {
            id: editor
        }
    }
}