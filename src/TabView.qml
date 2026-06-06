import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15 

Item {
    id: root

    property QtObject tabManager
    property QtObject theme

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
            color: root.theme.bgTabBar
            border.color: root.theme.borderPrimary
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
                                color: tabItem.isActive ? root.theme.bgTabActive : (tabHover.hovered ? root.theme.bgTabHover : root.theme.bgTabBar)
                                border.color: tabItem.isActive ? root.theme.borderPrimary : root.theme.borderInactive
                                border.width: 1

                                Rectangle {
                                    anchors.bottom: parent.bottom
                                    anchors.left: parent.left
                                    anchors.right: parent.right
                                    height: 2
                                    color: root.theme.accentBlue
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
                                color: tabItem.isActive ? root.theme.textPrimary : root.theme.textSecondary
                                font.pixelSize: root.theme.fontSizeNormal
                                font.family: root.theme.fontFamily
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
                                    color: closeBtnHover.hovered ? root.theme.closeBtnHover : "transparent"
                                }

                                Text {
                                    anchors.centerIn: parent
                                    text: tabModified ? "●" : "✕"
                                    color: tabModified ? root.theme.accentOrange : root.theme.textMuted
                                    font.family: root.theme.fontFamily
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
                    theme: root.theme
                    tabEditor: editor
                }
            }
        }
    }
}