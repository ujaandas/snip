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
            // Styling
            width: parent.width

            wrapMode: TextArea.NoWrap
            font.family: "JetBrains Mono"
            font.pixelSize: 14
            
            anchors.fill: parent

            // Data
            text: controller ? controller.text : ""

            onEditingFinished: {
                controller.text = text
            }

            Connections {
                target: controller
                function onTextChanged() {
                    if (!controller) 
                        return

                    if (editor.text !== controller.text) 
                        editor.text = controller.text
                }
            }
        }
    }
}