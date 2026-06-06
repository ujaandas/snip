import QtQuick
import QtQuick.Controls

Item {
    id: root
    property QtObject tabEditor

    EditorShortcuts {
        editor: tabEditor
    }

    Rectangle {
        anchors.fill: parent
        color: "#1f2430"
        border.color: "#2b313d"
        border.width: 1

        EditorViewport {
            anchors.fill: parent
            tabEditor: root.tabEditor
        }
    }
}