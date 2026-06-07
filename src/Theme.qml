pragma Singleton
import QtQuick

QtObject {
    // Fonts
    property string fontFamily: "JetBrains Mono"
    property int fontSizeSmall: 11
    property int fontSizeNormal: 12
    property int fontSizeMedium: 13
    property int fontSizeLarge: 14

    // Catppuccin Mocha - Base tones
    property color bgWindow: "#1e1e2e"      // Base
    property color bgSidebar: "#181825"     // Mantle
    property color bgHeader: "#181825"      // Mantle
    property color bgTabBar: "#11111b"      // Crust
    property color bgTabActive: "#1e1e2e"   // Base
    property color bgTabHover: "#313244"    // Surface0
    property color bgEditor: "#1e1e2e"      // Base
    property color bgGutter: "#181825"      // Mantle
    property color bgCursorLine: "#313244"  // Surface0

    // Catppuccin Mocha - Surface tones for borders
    property color borderPrimary: "#45475a"   // Surface1
    property color borderInactive: "#313244"  // Surface0

    // Catppuccin Mocha - Text tones
    property color textPrimary: "#cdd6f4"     // Text
    property color textSecondary: "#bac2de"   // Subtext1
    property color textMuted: "#a6adc8"       // Subtext0
    property color textDisabled: "#6c7086"    // Overlay0
    property color textTree: "#bac2de"        // Subtext1
    property color textTreeActive: "#cdd6f4"  // Text

    // Catppuccin Mocha - Accent colors
    property color accentBlue: "#89b4fa"        // Blue
    property color accentBlueLight: "#b4befe"   // Lavender
    property color accentBlueDark: "#1e1e2e"    // Base
    property color accentOrange: "#fab387"      // Peach

    // Catppuccin Mocha - UI elements
    property color scrollbar: "#585b70"         // Surface2
    property color selectionBg: "#45475a"       // Surface1
    property color selectionFg: "#cdd6f4"       // Text
    property color closeBtnHover: "#313244"     // Surface0
    property color treeChevron: "#7f849c"       // Overlay1
    property color treeBullet: "#6c7086"        // Overlay0
}
