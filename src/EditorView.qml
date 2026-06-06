import QtQuick
import QtQuick.Controls

Item {
    id: root
    property QtObject tabEditor

    EditorShortcuts {
        editor: tabEditor
    }

    Rectangle {
        anchors.fill: parent
        color: "#1f2430"
        border.color: "#2b313d"
        border.width: 1

        Row {
            anchors.fill: parent
            spacing: 0

            Gutter {
                id: gutter
                height: parent.height
                textArea: viewport.textArea
                scrollY: viewport.scrollY
                cursorLine: viewport.cursorLine
            }

            EditorViewport {
                id: viewport
                height: parent.height
                width: parent.width - gutter.width
                tabEditor: root.tabEditor
            }
        }
    }
}