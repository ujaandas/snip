import QtQuick
import QtQuick.Controls

ApplicationWindow {
    id: window
    visible: true
    width: 900
    height: 600
    flags: Qt.Window
    color: "#161a22"

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
        }

        FileTreeView {
            SplitView.preferredWidth: 250
            tree: fileTree
        }
    }
}