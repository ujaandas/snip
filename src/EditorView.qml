import QtQuick
import QtQuick.Controls

Item {
    id: root
    property QtObject controller   // Editor*

    ScrollView {
        anchors.fill: parent
        
        TextArea {
            // ID
            id: editor

            // Styling
            width: parent.width

            wrapMode: TextArea.NoWrap
            font.family: "JetBrains Mono"
            font.pixelSize: 14
            
            anchors.fill: parent

            // Data
            text: controller.text

            onEditingFinished: {
                controller.text = text
            }

            Connections {
                target: controller
                function onTextChanged() {
                    if (editor.text !== controller.text) {
                        editor.text = controller.text
                    }
                }
            }
        }
    }
}