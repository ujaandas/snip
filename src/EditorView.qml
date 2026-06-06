import QtQuick
import QtQuick.Controls

Item {
    id: root
    property QtObject controller   // Editor*

    EditorShortcuts {
        editor: controller
    }

    Rectangle {
        anchors.fill: parent
        color: "#1e1e1e"

        ScrollView {
            anchors.fill: parent
            clip: true

            TextArea {
                id: editor

                // data
                Component.onCompleted: controller.setQuickDocument(editor.textDocument)

                // layout
                width: parent.width
                height: contentHeight
                anchors.fill: parent

                // editor behavior
                wrapMode: TextArea.NoWrap

                // font
                font.family: "JetBrains Mono"
                font.pixelSize: 14

                // colours
                color: "#d4d4d4"
                selectionColor: "#264f78"
                selectedTextColor: "#ffffff"

                background: Rectangle {
                    color: "#1e1e1e"
                }

                leftPadding: 12
                rightPadding: 12
                topPadding: 10
                bottomPadding: 10

                cursorDelegate: Rectangle {
                    width: 2
                    color: "#aeafad"
                }
            }
        }
    }
}