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

            required property TreeView treeView
            required property bool isTreeNode
            required property bool expanded
            required property bool hasChildren
            required property int depth
            required property int row
            required property int column

            required property string fileName
            required property string filePath

            TapHandler {
                onTapped: {
                    if (hasChildren) {
                        treeView.toggleExpanded(row)
                    } else {
                        tabManager.openTab(fileName, filePath)
                    }
                }
            }

            Row {
                anchors.verticalCenter: parent.verticalCenter
                x: depth * 16 + 4
                spacing: 4

                Text {
                    visible: hasChildren
                    text: expanded ? "▾" : "▸"
                    color: "white"
                    font.pixelSize: 13
                }

                Text {
                    text: display
                    elide: Text.ElideRight
                    color: "white" 
                    font.pixelSize: 13
                }
            }
        }
    }
}