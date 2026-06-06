import QtQuick
import QtQuick.Controls

Item {
    id: root

    property QtObject tree
    property QtObject tabManager
    property QtObject theme

    Rectangle {
        anchors.fill: parent
        color: root.theme.bgSidebar
        border.color: root.theme.borderPrimary
        border.width: 1

        Rectangle {
            id: header
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.right: parent.right
            height: 34
            color: root.theme.bgHeader
            border.color: root.theme.borderPrimary
            border.width: 1

            Text {
                anchors.left: parent.left
                anchors.leftMargin: 12
                anchors.verticalCenter: parent.verticalCenter
                text: "Explorer"
                color: root.theme.textMuted
                font.family: root.theme.fontFamily
                font.pixelSize: root.theme.fontSizeSmall
                font.letterSpacing: 1.2
            }
        }

        TreeView {
            id: treeView
            anchors.top: header.bottom
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            rootIndex: root.tree ? root.tree.rootIndex : undefined
            model: root.tree ? root.tree.model : null
            clip: true

            columnWidthProvider: function(col) { return col === 0 ? -1 : 0 }

            delegate: Item {
                id: delegateItem
                implicitHeight: 26
                implicitWidth: Math.max(treeView.width, depth * 14 + 8 + rowContents.implicitWidth + 12)

                visible: column === 0

                required property TreeView treeView
                required property bool isTreeNode
                required property bool expanded
                required property bool hasChildren
                required property int depth
                required property int row
                required property int column

                required property string fileName
                required property string filePath

                readonly property bool isActive: !hasChildren && (filePath === root.tabManager?.activeFilePath)

                MouseArea {
                    id: mouseArea
                    anchors.fill: parent
                    hoverEnabled: true
                    onClicked: {
                        if (hasChildren) {
                            delegateItem.treeView.toggleExpanded(row)
                        } else {
                            root.tabManager.openTab(fileName, filePath)
                        }
                    }
                }

                Rectangle {
                    anchors.fill: parent
                    color: isActive
                        ? root.theme.accentBlueDark
                        : mouseArea.containsMouse ? "#222839" : "transparent"
                }

                Rectangle {
                    visible: isActive
                    x: 0
                    y: 0
                    width: 2
                    height: parent.height
                    color: root.theme.accentBlueLight
                }

                Row {
                    id: rowContents
                    anchors.verticalCenter: parent.verticalCenter
                    x: depth * 14 + 8
                    spacing: 5

                    Text {
                        visible: hasChildren
                        text: expanded ? "▾" : "▸"
                        color: root.theme.treeChevron
                        font.pixelSize: root.theme.fontSizeNormal
                        font.family: root.theme.fontFamily
                        anchors.verticalCenter: parent.verticalCenter
                    }

                    Text {
                        visible: !hasChildren
                        text: "•"
                        color: root.theme.treeBullet
                        font.pixelSize: 9
                        anchors.verticalCenter: parent.verticalCenter
                    }

                    Text {
                        text: display
                        color: isActive ? root.theme.textTreeActive : root.theme.textTree
                        font.pixelSize: root.theme.fontSizeMedium
                        font.family: root.theme.fontFamily
                    }
                }
            }
        }
    }
}