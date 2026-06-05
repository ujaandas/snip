import QtQuick
import QtQuick.Controls

Item {
    id: root
    property QtObject controller   // Editor*

    ScrollView {
        anchors.fill: parent
        
        TextArea {
            id: editor
            width: parent.width

            wrapMode: TextArea.NoWrap
            font.family: "JetBrains Mono"
            font.pixelSize: 14

            anchors.fill: parent
            text: controller.text

            onTextChanged: controller.text = text
        }
    }
}