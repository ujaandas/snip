import QtQuick
import QtQuick.Controls

Item {
    id: root

    property QtObject controller   // FileTreeController*

    ScrollView {
        anchors.fill: parent

        TreeView {
            id: tree

            anchors.fill: parent

            delegate: Item {
                implicitHeight: 24
                implicitWidth: tree.width

                Text {
                    anchors.verticalCenter: parent.verticalCenter
                    color: "white"
                    font.pixelSize: 13
                    elide: Text.ElideRight
                }
            }
        }
    }
}