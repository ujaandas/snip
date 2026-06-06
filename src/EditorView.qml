import QtQuick
import QtQuick.Controls

Item {
    id: root
    property QtObject tabEditor
    property QtObject theme

    Rectangle {
        anchors.fill: parent
        color: root.theme.bgEditor
        border.color: root.theme.borderPrimary
        border.width: 1

        Row {
            anchors.fill: parent
            spacing: 0

            Gutter {
                id: gutter
                height: parent.height
                theme: root.theme
                textArea: viewport.textArea
                scrollY: viewport.scrollY
            }

            EditorViewport {
                id: viewport
                height: parent.height
                width: parent.width - gutter.width
                theme: root.theme
                tabEditor: root.tabEditor
            }
        }
    }
}