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

        SplitView {
            orientation: Qt.Vertical
            SplitView.fillWidth: true
            SplitView.fillHeight: true
            handle: Rectangle {
                implicitWidth: 1
                implicitHeight: 1
                color: "#2b313d"
            }

            TabView {
                SplitView.fillWidth: true
                SplitView.fillHeight: true
            }

            Rectangle {
                color: "#171c26"
                border.color: "#2b313d"
                border.width: 1
                SplitView.preferredHeight: 150

                Text {
                    anchors.left: parent.left
                    anchors.leftMargin: 12
                    anchors.verticalCenter: parent.verticalCenter
                    text: "Output"
                    color: "#9aa4b5"
                    font.family: "JetBrains Mono"
                    font.pixelSize: 12
                }
            }
        }

        FileTreeView {
            SplitView.preferredWidth: 250
            tree: fileTree
        }
    }
}