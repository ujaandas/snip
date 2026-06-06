import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15 

Item {
    id: root

    Column {
        anchors.fill: parent
        spacing: 0

        TabBar {
            id: tabBar
            width: parent.width
            height: 40
            spacing: 2
            padding: 6

            background: Rectangle {
                color: "#171c26"
                border.color: "#2b313d"
                border.width: 1
            }

            currentIndex: tabManager.activeTab
            onCurrentIndexChanged: tabManager.activeTab = currentIndex

            Repeater {
                model: tabManager
                
                TabButton {
                    text: tabTitle
                    implicitHeight: 28
                    implicitWidth: Math.max(140, contentItem.implicitWidth + 28)
                    leftPadding: 12
                    rightPadding: 30
                    hoverEnabled: true

                    background: Rectangle {
                        radius: 6
                        color: checked ? "#242b38" : (parent.hovered ? "#1f2532" : "transparent")
                        border.color: checked ? "#3a4456" : "transparent"
                        border.width: 1
                    }

                    contentItem: Text {
                        text: parent.text
                        elide: Text.ElideRight
                        verticalAlignment: Text.AlignVCenter
                        color: parent.checked ? "#e6ecf8" : "#a9b2c3"
                        font.pixelSize: 12
                        font.family: "JetBrains Mono"
                    }
                    
                    Button {
                        text: "x"
                        width: 18
                        height: 18
                        anchors.right: parent.right
                        anchors.rightMargin: 6
                        anchors.verticalCenter: parent.verticalCenter
                        background: Rectangle {
                            radius: 4
                            color: parent.hovered ? "#3a4152" : "transparent"
                        }
                        contentItem: Text {
                            text: parent.text
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter
                            color: "#9aa4b5"
                            font.family: "JetBrains Mono"
                            font.pixelSize: 12
                        }
                        onClicked: tabManager.closeTab(index)
                    }
                }
            }
        }

        StackLayout {
            width: parent.width
            height: parent.height - tabBar.height

            currentIndex: tabManager.activeTab

            Repeater {
                model: tabManager
                
                EditorView {
                    SplitView.fillHeight: true
                    controller: editorController
                }
            }
        }
    }
}