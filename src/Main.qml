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
            SplitView.fillWidth: true
            orientation: Qt.Vertical

            EditorView {
                SplitView.fillHeight: true
                controller: editor
            }

            Rectangle {
                SplitView.preferredHeight: 150
                color: "#111111" 
            }
        }

        FileTreeView {
            SplitView.preferredWidth: 250
            controller: fileTree
        }
    }
}