import QtQuick
import QtQuick.Controls
import Snip.Editor 1.0

StrictScroll {
    id: viewport

    property QtObject tabEditor
    property alias textArea: editorArea
    property alias scrollY: viewport.scrollY
    property int cursorLine: {
        if (!editorArea.textDocument) return -1
        var pos = editorArea.cursorPosition
        var text = editorArea.text.substring(0, pos)
        return text.split("\n").length - 1
    }

    content: TextArea {
        id: editorArea

        width: Math.max(implicitWidth, viewport.width)
        height: Math.max(implicitHeight, viewport.height)

        Component.onCompleted: {
            if (tabEditor) tabEditor.setQuickDocument(editorArea.textDocument)
            editorArea.cursorPosition = 0
        }

        wrapMode: TextArea.NoWrap

        font.family: "JetBrains Mono"
        font.pixelSize: 14

        color: "#cfd6e4"
        selectionColor: "#214b78"
        selectedTextColor: "#ffffff"

        background: Rectangle {
            color: "#1f2430"

            Rectangle {
                x: 0
                y: {
                    var r = editorArea.positionToRectangle(editorArea.cursorPosition)
                    return r.y - viewport.scrollY
                }
                width: parent.width
                height: {
                    var r = editorArea.positionToRectangle(editorArea.cursorPosition)
                    return r.height
                }
                color: "#2a3040"
                visible: editorArea.activeFocus
            }
        }

        leftPadding: 16
        rightPadding: 16
        topPadding: 12
        bottomPadding: 12

        cursorDelegate: Rectangle {
            width: 2
            height: fontMetrics.height
            color: "#99c4ff"

            FontMetrics {
                id: fontMetrics
                font: editorArea.font
            }
        }
    }
}