import QtQuick
import QtQuick.Controls
import Snip.Editor 1.0

StrictScroll {
    id: viewport

    property QtObject tabEditor
    property alias textArea: editorArea
    property alias scrollY: viewport.scrollY

    property bool _scrollbarVisible: false
    property int indentSize: 4
    property bool wordWrap: false

    readonly property int cursorLine: {
        if (!editorArea.text) return 1
        return editorArea.text.substring(0, editorArea.cursorPosition).split("\n").length
    }
    readonly property int cursorColumn: {
        if (!editorArea.text) return 1
        var pos = editorArea.cursorPosition
        var textBefore = editorArea.text.substring(0, pos)
        var lastNewline = textBefore.lastIndexOf("\n")
        return pos - lastNewline
    }

    Timer {
        id: scrollbarFadeTimer
        interval: 1200
        onTriggered: viewport._scrollbarVisible = false
    }

    onScrollYChanged: { _scrollbarVisible = true; scrollbarFadeTimer.restart() }
    onScrollXChanged: { _scrollbarVisible = true; scrollbarFadeTimer.restart() }

    Rectangle {
        id: vScrollbar
        z: 10
        anchors.right: parent.right
        anchors.rightMargin: 2
        width: 6
        color: "#4a5268"

        visible: viewport.vThumbNeeded
        opacity: (hoverHandler.hovered || dragHandler.active || viewport._scrollbarVisible) ? 0.8 : 0
        Behavior on opacity { NumberAnimation { duration: 150 } }

        y: viewport.vThumbY
        height: viewport.vThumbHeight

        HoverHandler { id: hoverHandler }

        DragHandler {
            id: dragHandler
            target: null
            property real startScrollY: 0
            onActiveChanged: if (active) startScrollY = viewport.scrollY
            onTranslationChanged: viewport.dragScrollY(startScrollY, translation.y, vScrollbar.height)
        }
    }

    Rectangle {
        id: hScrollbar
        z: 10
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 2
        height: 6
        color: "#4a5268"

        visible: viewport.hThumbNeeded
        opacity: (hHoverHandler.hovered || hDragHandler.active || viewport._scrollbarVisible) ? 0.8 : 0
        Behavior on opacity { NumberAnimation { duration: 150 } }

        x: viewport.hThumbX
        width: viewport.hThumbWidth

        HoverHandler { id: hHoverHandler }

        DragHandler {
            id: hDragHandler
            target: null
            property real startScrollX: 0
            onActiveChanged: if (active) startScrollX = viewport.scrollX
            onTranslationChanged: viewport.dragScrollX(startScrollX, translation.x, hScrollbar.width)
        }
    }

    content: TextArea {
        id: editorArea

        width: Math.max(implicitWidth, viewport.width)
        height: Math.max(implicitHeight, viewport.height)

        Component.onCompleted: {
            if (tabEditor) tabEditor.setQuickDocument(editorArea.textDocument)
            editorArea.cursorPosition = 0
        }

        wrapMode: viewport.wordWrap ? TextArea.Wrap : TextArea.NoWrap

        font.family: "JetBrains Mono"
        font.pixelSize: 14

        color: "#cfd6e4"
        selectionColor: "#214b78"
        selectedTextColor: "#ffffff"

        background: Rectangle {
            color: "#1f2430"

            Rectangle {
                x: 0
                y: editorArea.positionToRectangle(editorArea.cursorPosition).y
                width: parent.width
                height: editorArea.positionToRectangle(editorArea.cursorPosition).height
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