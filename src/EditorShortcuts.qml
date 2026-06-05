import QtQuick
import QtQuick.Controls

Item {
    property QtObject editor

    Shortcut {
        sequence: StandardKey.Save
        onActivated: editor.save()
    }

    Shortcut {
        sequence: StandardKey.Undo
        onActivated: editor.undo()
    }

    Shortcut {
        sequence: StandardKey.Redo
        onActivated: editor.redo()
    }
}