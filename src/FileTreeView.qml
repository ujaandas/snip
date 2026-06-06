import QtQuick
import QtQuick.Controls

Item {
    id: root

    property QtObject controller   // FileTreeController*

    Rectangle {
        anchors.fill: parent
        color: "#181d27"
        border.color: "#2b313d"
        border.width: 1

        Rectangle {
            id: header
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.right: parent.right
            height: 34
            color: "#161b24"
            border.color: "#2b313d"
            border.width: 1

            Text {
                anchors.left: parent.left
                anchors.leftMargin: 12
                anchors.verticalCenter: parent.verticalCenter
                text: "Explorer"
                color: "#9aa4b5"
                font.family: "JetBrains Mono"
                font.pixelSize: 12
            }
        }

        TreeView {
            id: tree
            anchors.top: header.bottom
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            rootIndex: controller.rootIndex
            model: controller.model
            clip: true

            Rectangle {
                anchors.fill: parent
                color: "transparent"
            }

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

                HoverHandler {
                    id: hoverHandler
                }

                Rectangle {
                    anchors.fill: parent
                    color: hoverHandler.hovered ? "#262d3a" : "transparent"
                }

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
                    x: depth * 14 + 8
                    spacing: 6

                    Text {
                        visible: hasChildren
                        text: expanded ? "▾" : "▸"
                        color: "#8f99aa"
                        font.pixelSize: 12
                        font.family: "JetBrains Mono"
                    }

                    Text {
                        visible: !hasChildren
                        text: "•"
                        color: "#6e7787"
                        font.pixelSize: 9
                        anchors.verticalCenter: parent.verticalCenter
                    }

                    Text {
                        text: display
                        elide: Text.ElideRight
                        color: "#d7deeb"
                        font.pixelSize: 13
                        font.family: "JetBrains Mono"
                    }
                }
            }
        }
    }
}