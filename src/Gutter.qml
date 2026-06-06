import QtQuick
import Snip.Editor 1.0

Rectangle {
    id: root

    property var textArea: null
    property QtObject theme
    property real scrollY: 0

    width: fontMetrics.averageCharacterWidth * controller.digitCount + 28
    clip: true
    color: root.theme.bgGutter
    border.color: root.theme.borderPrimary
    border.width: 1

    Gutter {
        id: controller
        textDocument: root.textArea?.textDocument ?? null
        cursorPosition: root.textArea?.cursorPosition ?? -1
    }

    FontMetrics {
        id: fontMetrics
        font: root.textArea?.font ?? ({ family: root.theme.fontFamily, pixelSize: root.theme.fontSizeLarge })
    }

    Repeater {
        model: controller.lineCount

        Text {
            x: 0
            y: controller.lineYPosition(index) - root.scrollY + (root.textArea?.topPadding ?? 12)
            width: root.width - 10
            height: Math.max(fontMetrics.height, controller.lineHeight(index))
            horizontalAlignment: Text.AlignRight
            verticalAlignment: Text.AlignVCenter

            text: index + 1
            font: fontMetrics.font
            color: index === controller.cursorLine ? root.theme.textPrimary : root.theme.textDisabled
        }
    }
}
