pragma Singleton
import QtQuick

QtObject {
    // Fonts
    property string fontFamily: "JetBrains Mono"
    property int fontSizeSmall: 11
    property int fontSizeNormal: 12
    property int fontSizeMedium: 13
    property int fontSizeLarge: 14

    // Background colors
    property color bgWindow: "#161a22"
    property color bgSidebar: "#181d27"
    property color bgHeader: "#161b24"
    property color bgTabBar: "#171c26"
    property color bgTabActive: "#1f2430"
    property color bgTabHover: "#1b202b"
    property color bgEditor: "#1f2430"
    property color bgGutter: "#1a1e26"
    property color bgCursorLine: "#2a3040"

    // Border colors
    property color borderPrimary: "#2b313d"
    property color borderInactive: "#171c26"

    // Text colors
    property color textPrimary: "#e6ecf8"
    property color textSecondary: "#a9b2c3"
    property color textMuted: "#9aa4b5"
    property color textDisabled: "#4e5668"
    property color textTree: "#d7deeb"
    property color textTreeActive: "#e8edf5"

    // Accent colors
    property color accentBlue: "#4d9ef5"
    property color accentBlueLight: "#99c4ff"
    property color accentBlueDark: "#1e3a5f"
    property color accentOrange: "#e6bd6a"

    // Scrollbar
    property color scrollbar: "#4a5268"

    // Selection
    property color selectionBg: "#214b78"
    property color selectionFg: "#ffffff"

    // Close button
    property color closeBtnHover: "#3a4152"

    // Tree view
    property color treeChevron: "#8f99aa"
    property color treeBullet: "#6e7787"
}
