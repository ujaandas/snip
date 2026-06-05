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

            wrapMode: TextArea.NoWrap
            font.family: "JetBrains Mono"
            font.pixelSize: 14
            
            anchors.fill: parent

            // Data
            text: controller ? controller.text : ""

            onTextChanged: {
                if (controller && controller.text !== text)
                    controller.text = text
            }
        }
    }
}