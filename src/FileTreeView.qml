import QtQuick
import QtQuick.Controls

Item {
    id: root

    property QtObject controller   // FileTreeController*

    TreeView {
        id: tree
        anchors.fill: parent
        rootIndex: controller.rootIndex
        model: controller.model

        delegate: Item {
            implicitHeight: 24
            implicitWidth: tree.width

            Text {
                text: model.display
                anchors.verticalCenter: parent.verticalCenter
                color: "white"
                font.pixelSize: 13
                elide: Text.ElideRight
            }
        }
    }
}