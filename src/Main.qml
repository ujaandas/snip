import QtQuick
import QtQuick.Controls

ApplicationWindow {
    id: root
    visible: true
    width: 900
    height: 600
    flags: Qt.Window
    color: root.appTheme.bgWindow

    property QtObject tabs: tabManager
    property QtObject files: fileTree
    property QtObject appTheme: theme

    SplitView {
        anchors.fill: parent
        orientation: Qt.Horizontal
        handle: Rectangle {
            implicitWidth: 1
            implicitHeight: 1
            color: root.appTheme.borderPrimary
        }

        TabView {
            SplitView.fillWidth: true
            SplitView.fillHeight: true
            tabManager: root.tabs
            theme: root.appTheme
        }

        FileTreeView {
            SplitView.preferredWidth: 250
            tree: root.files
            tabManager: root.tabs
            theme: root.appTheme
        }
    }
}