import QtQuick
import QtQuick.Controls

ApplicationWindow {
    id: root
    visible: true
    width: 900
    height: 600
    flags: Qt.Window
    color: "#161a22"

    property QtObject tabs: tabManager
    property QtObject files: fileTree

    SplitView {
        anchors.fill: parent
        orientation: Qt.Horizontal
        handle: Rectangle {
            implicitWidth: 1
            implicitHeight: 1
            color: "#2b313d"
        }

        TabView {
            SplitView.fillWidth: true
            SplitView.fillHeight: true
            tabManager: root.tabs
        }

        FileTreeView {
            SplitView.preferredWidth: 250
            tree: root.files
            tabManager: root.tabs
        }
    }
}