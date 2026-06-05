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
                controller.setText(text)
            }

            Connections {
                target: controller
                function onTextChanged() {
                    if (!editor.activeFocus) {
                        editor.text = controller.text
                    }
                }
            }
        }
    }
}