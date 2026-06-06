import QtQuick
import Snip.Editor 1.0

Rectangle {
    id: root

    property var textArea: null
    property real scrollY: 0
    property int cursorLine: -1

    readonly property int digitCount: Math.max(2, controller.lineCount.toString().length)
    width: fontMetrics.averageCharacterWidth * digitCount + 28
    clip: true
    color: "#1a1e26"
    border.color: "#2b313d"
    border.width: 1

    Gutter {
        id: controller
        textDocument: root.textArea ? root.textArea.textDocument : null
    }

    FontMetrics {
        id: fontMetrics
        font: root.textArea ? root.textArea.font : ({ family: "JetBrains Mono", pixelSize: 14 })
    }

    Repeater {
        model: controller.lineCount

        Text {
            x: 0
            y: controller.lineYPosition(index) - root.scrollY + (root.textArea ? root.textArea.topPadding : 12)
            width: root.width - 10
            height: Math.max(fontMetrics.height, controller.lineHeight(index))
            horizontalAlignment: Text.AlignRight
            verticalAlignment: Text.AlignVCenter

            text: index + 1
            font: fontMetrics.font
            color: index === root.cursorLine ? "#c8cdd5" : "#4e5668"
        }
    }
}
