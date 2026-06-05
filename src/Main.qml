import QtQuick
import QtQuick.Window
import MyEditor 1.0

Window {
    width: 480
    height: 240
    visible: true
    title: "snip"

    Flickable {
        id: flick
        anchors.fill: parent
        clip: true

        contentWidth: editor.documentWidth
        contentHeight: editor.documentHeight

        EditorView {
            id: editor
            width: flick.width
            height: editor.documentHeight
        }
    }
}