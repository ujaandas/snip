import QtQuick
import QtQuick.Window
import MyScroll 1.0
import MyEditor 1.0

Window {
    width: 800
    height: 600
    visible: true

    StrictScrollViewport {
        anchors.fill: parent

        content: EditorView {
            width: contentWidth
            height: contentHeight
        }
    }
}