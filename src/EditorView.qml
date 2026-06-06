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

        ScrollView {
            anchors.fill: parent
            clip: true

            ScrollBar.vertical: ScrollBar {
                policy: ScrollBar.AsNeeded
                width: 10
                contentItem: Rectangle {
                    radius: 5
                    color: parent.pressed ? "#5b6474" : "#464e5d"
                    opacity: 0.9
                }
                background: Rectangle {
                    color: "#1a1f2b"
                }
            }

            ScrollBar.horizontal: ScrollBar {
                policy: ScrollBar.AsNeeded
                height: 10
                contentItem: Rectangle {
                    radius: 5
                    color: parent.pressed ? "#5b6474" : "#464e5d"
                    opacity: 0.9
                }
                background: Rectangle {
                    color: "#1a1f2b"
                }
            }

            TextArea {
                id: editorArea

                // data
                Component.onCompleted: {
                    if (tabEditor) tabEditor.setQuickDocument(editorArea.textDocument)
                    editorArea.cursorPosition = 0
                }

                // editor behavior
                wrapMode: TextArea.NoWrap

                // font
                font.family: "JetBrains Mono"
                font.pixelSize: 14

                // colours
                color: "#cfd6e4"
                selectionColor: "#214b78"
                selectedTextColor: "#ffffff"

                background: Rectangle {
                    color: "#1f2430"
                }

                leftPadding: 16
                rightPadding: 16
                topPadding: 12
                bottomPadding: 12

                cursorDelegate: Rectangle {
                    width: 2
                    color: "#99c4ff"
                }
            }
        }
    }
}