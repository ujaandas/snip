import QtQuick
import QtQuick.Controls

Item {
    id: root
    property QtObject controller   // Editor*

    EditorShortcuts {
        editor: controller
    }

    ScrollView {
        anchors.fill: parent
        
        TextArea {
            id: editor

            // Styling
            width: parent.width
            height: contentHeight
            anchors.fill: parent

            wrapMode: TextArea.NoWrap
            font.family: "JetBrains Mono"
            font.pixelSize: 14

            // Data
            Component.onCompleted: controller.setQuickDocument(editor.textDocument)
        }
    }
}