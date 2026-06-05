import QtQuick
import QtQuick.Controls

Item {
    property QtObject editor

    Shortcut {
        sequences: [ StandardKey.Save ]
        onActivated: editor.save()
    }

    Shortcut {
        sequences: [ StandardKey.Undo ]
        onActivated: editor.undo()
    }

    Shortcut {
        sequences: [ StandardKey.Redo ]
        onActivated: editor.redo()
    }
}