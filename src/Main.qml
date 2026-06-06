import QtQuick
import QtQuick.Controls

ApplicationWindow {
    visible: true
    width: 900
    height: 600

    SplitView {
        anchors.fill: parent
        orientation: Qt.Horizontal

        SplitView {
            orientation: Qt.Vertical
            SplitView.fillWidth: true
            SplitView.fillHeight: true

            TabView {
                SplitView.fillWidth: true
                SplitView.fillHeight: true
            }

            Rectangle {
                color: "#111111"
                SplitView.preferredHeight: 150
            }
        }

        FileTreeView {
            SplitView.preferredWidth: 250
            controller: fileTree
        }
    }
}