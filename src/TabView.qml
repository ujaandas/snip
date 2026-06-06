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
            height: 36
            spacing: 0
            padding: 0

            background: Rectangle {
                color: "#171c26"
                border.color: "#2b313d"
                border.width: 1
            }

            currentIndex: tabManager ? tabManager.activeTab : -1
            onCurrentIndexChanged: tabManager ? tabManager.activeTab = currentIndex : null

            Repeater {
                model: tabManager
                
                TabButton {
                    checked: index === tabManager ? tabManager.activeTab : -1

                    text: tabTitle
                    implicitHeight: 35
                    implicitWidth: Math.max(140, contentItem.implicitWidth + 28)
                    leftPadding: 12
                    rightPadding: 30
                    hoverEnabled: true

                    background: Rectangle {
                        radius: 0
                        color: checked ? "#1f2430" : (parent.hovered ? "#1b202b" : "#171c26")
                        border.color: checked ? "#2b313d" : "#171c26"
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
                            radius: 0
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

            currentIndex: tabManager ? tabManager.activeTab : -1

            Repeater {
                model: tabManager
                
                EditorView {
                    SplitView.fillHeight: true
                    tabEditor: editor
                }
            }
        }
    }
}