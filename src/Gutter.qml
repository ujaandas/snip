import QtQuick
import Snip.Editor 1.0

Rectangle {
    id: root

    property var textArea: null
    property real scrollY: 0

    width: 50
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
            width: root.width
            height: Math.max(fontMetrics.height, controller.lineHeight(index))
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter

            text: index + 1
            font: fontMetrics.font
            color: "#6b7280"
        }
    }
}
