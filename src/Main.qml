import QtQuick
import QtQuick.Window
import QtQuick.Controls
import MyEditor 1.0

Window {
    width: 480
    height: 240
    visible: true
    title: "snip"

    EditorView {
        anchors.fill: parent
        id: editor
        width: editor.documentWidth
        height: editor.documentHeight
    }
}