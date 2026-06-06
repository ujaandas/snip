import QtQuick
import QtQuick.Controls

Item {
    property QtObject tabManager

    Shortcut {
        sequences: [ StandardKey.Save ]
        onActivated: tabManager?.activeEditor?.save()
    }

    Shortcut {
        sequences: [ StandardKey.Undo ]
        onActivated: tabManager?.activeEditor?.undo()
    }

    Shortcut {
        sequences: [ StandardKey.Redo ]
        onActivated: tabManager?.activeEditor?.redo()
    }

    Shortcut {
        sequences: [ StandardKey.Close ]
        onActivated: tabManager?.closeActiveTab()
    }

    Shortcut {
        sequence: "Ctrl+Tab"
        onActivated: tabManager?.nextTab()
    }

    Shortcut {
        sequence: "Ctrl+Shift+Tab"
        onActivated: tabManager?.prevTab()
    }
}