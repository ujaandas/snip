import QtQuick
import QtQuick.Controls
import Snip.Editor 1.0

StrictScroll {
    id: viewport

    property QtObject tabEditor

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