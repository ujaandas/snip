import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15 

Item {
    id: root

    property QtObject tabManager

    EditorShortcuts {
        tabManager: root.tabManager
    }

    Column {
        anchors.fill: parent
        spacing: 0

        Rectangle {
            id: tabBar
            width: parent.width
            height: 36
            color: "#171c26"
            border.color: "#2b313d"
            border.width: 1
            clip: true

            Flickable {
                id: tabFlickable
                anchors.fill: parent
                contentWidth: tabRow.width
                contentHeight: height
                clip: true
                flickableDirection: Flickable.HorizontalFlick
                interactive: tabRow.width > tabBar.width

                onContentXChanged: {
                    if (contentX < 0) contentX = 0
                    if (contentX > contentWidth - width) contentX = Math.max(0, contentWidth - width)
                }

                Row {
                    id: tabRow
                    height: tabBar.height
                    spacing: 0

                    Repeater {
                        id: tabRepeater
                        model: root.tabManager

                        Item {
                            id: tabItem
                            property bool isActive: index === root.tabManager?.activeTab
                            width: Math.max(140, tabLabel.implicitWidth + 52)
                            height: tabBar.height

                            onIsActiveChanged: {
                                if (!isActive) return
                                var left = tabItem.x
                                var right = tabItem.x + tabItem.width
                                if (left < tabFlickable.contentX)
                                    tabFlickable.contentX = left
                                else if (right > tabFlickable.contentX + tabFlickable.width)
                                    tabFlickable.contentX = right - tabFlickable.width
                            }

                            HoverHandler { id: tabHover }

                            TapHandler {
                                onTapped: root.tabManager.activeTab = index
                            }

                            Rectangle {
                                anchors.fill: parent
                                color: tabItem.isActive ? "#1f2430" : (tabHover.hovered ? "#1b202b" : "#171c26")
                                border.color: tabItem.isActive ? "#2b313d" : "#171c26"
                                border.width: 1

                                Rectangle {
                                    anchors.bottom: parent.bottom
                                    anchors.left: parent.left
                                    anchors.right: parent.right
                                    height: 2
                                    color: "#4d9ef5"
                                    visible: tabItem.isActive
                                }
                            }

                            Text {
                                id: tabLabel
                                anchors.left: parent.left
                                anchors.leftMargin: 12
                                anchors.verticalCenter: parent.verticalCenter
                                anchors.right: closeBtn.left
                                anchors.rightMargin: 4
                                text: tabTitle
                                elide: Text.ElideRight
                                color: tabItem.isActive ? "#e6ecf8" : "#a9b2c3"
                                font.pixelSize: 12
                                font.family: "JetBrains Mono"
                            }

                            Item {
                                id: closeBtn
                                width: 18
                                height: 18
                                anchors.right: parent.right
                                anchors.rightMargin: 6
                                anchors.verticalCenter: parent.verticalCenter

                                HoverHandler { id: closeBtnHover }
                                TapHandler { onTapped: root.tabManager.closeTab(index) }

                                Rectangle {
                                    anchors.fill: parent
                                    color: closeBtnHover.hovered ? "#3a4152" : "transparent"
                                }

                                Text {
                                    anchors.centerIn: parent
                                    text: tabModified ? "●" : "✕"
                                    color: tabModified ? "#e6bd6a" : "#9aa4b5"
                                    font.family: "JetBrains Mono"
                                    font.pixelSize: tabModified ? 10 : 12
                                }
                            }
                        }
                    }
                }
            }
        }

        StackLayout {
            width: parent.width
            height: parent.height - tabBar.height

            currentIndex: root.tabManager?.activeTab ?? -1

            Repeater {
                model: root.tabManager

                EditorView {
                    SplitView.fillHeight: true
                    tabEditor: editor
                }
            }
        }
    }
}