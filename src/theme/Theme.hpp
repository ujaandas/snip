#pragma once

#include <QColor>
#include <QObject>

class Theme : public QObject {
  Q_OBJECT;

  // Fonts
  Q_PROPERTY(QString fontFamily MEMBER fontFamily CONSTANT);
  Q_PROPERTY(int fontSizeSmall MEMBER fontSizeSmall CONSTANT);
  Q_PROPERTY(int fontSizeNormal MEMBER fontSizeNormal CONSTANT);
  Q_PROPERTY(int fontSizeMedium MEMBER fontSizeMedium CONSTANT);
  Q_PROPERTY(int fontSizeLarge MEMBER fontSizeLarge CONSTANT);

  // Background colors
  Q_PROPERTY(QColor bgWindow MEMBER bgWindow CONSTANT);
  Q_PROPERTY(QColor bgSidebar MEMBER bgSidebar CONSTANT);
  Q_PROPERTY(QColor bgHeader MEMBER bgHeader CONSTANT);
  Q_PROPERTY(QColor bgTabBar MEMBER bgTabBar CONSTANT);
  Q_PROPERTY(QColor bgTabActive MEMBER bgTabActive CONSTANT);
  Q_PROPERTY(QColor bgTabHover MEMBER bgTabHover CONSTANT);
  Q_PROPERTY(QColor bgEditor MEMBER bgEditor CONSTANT);
  Q_PROPERTY(QColor bgGutter MEMBER bgGutter CONSTANT);
  Q_PROPERTY(QColor bgCursorLine MEMBER bgCursorLine CONSTANT);

  // Border colors
  Q_PROPERTY(QColor borderPrimary MEMBER borderPrimary CONSTANT);
  Q_PROPERTY(QColor borderInactive MEMBER borderInactive CONSTANT);

  // Text colors
  Q_PROPERTY(QColor textPrimary MEMBER textPrimary CONSTANT);
  Q_PROPERTY(QColor textSecondary MEMBER textSecondary CONSTANT);
  Q_PROPERTY(QColor textMuted MEMBER textMuted CONSTANT);
  Q_PROPERTY(QColor textDisabled MEMBER textDisabled CONSTANT);
  Q_PROPERTY(QColor textTree MEMBER textTree CONSTANT);
  Q_PROPERTY(QColor textTreeActive MEMBER textTreeActive CONSTANT);

  // Accent colors
  Q_PROPERTY(QColor accentBlue MEMBER accentBlue CONSTANT);
  Q_PROPERTY(QColor accentBlueLight MEMBER accentBlueLight CONSTANT);
  Q_PROPERTY(QColor accentBlueDark MEMBER accentBlueDark CONSTANT);
  Q_PROPERTY(QColor accentOrange MEMBER accentOrange CONSTANT);

  // Other
  Q_PROPERTY(QColor scrollbar MEMBER scrollbar CONSTANT);
  Q_PROPERTY(QColor selectionBg MEMBER selectionBg CONSTANT);
  Q_PROPERTY(QColor selectionFg MEMBER selectionFg CONSTANT);
  Q_PROPERTY(QColor closeBtnHover MEMBER closeBtnHover CONSTANT);
  Q_PROPERTY(QColor treeChevron MEMBER treeChevron CONSTANT);
  Q_PROPERTY(QColor treeBullet MEMBER treeBullet CONSTANT);

  // Syntax highlighting - Tree-sitter
  Q_PROPERTY(QColor hlKeyword MEMBER hlKeyword CONSTANT);
  Q_PROPERTY(QColor hlOperator MEMBER hlOperator CONSTANT);
  Q_PROPERTY(QColor hlString MEMBER hlString CONSTANT);
  Q_PROPERTY(QColor hlComment MEMBER hlComment CONSTANT);
  Q_PROPERTY(QColor hlNumber MEMBER hlNumber CONSTANT);
  Q_PROPERTY(QColor hlPunctuation MEMBER hlPunctuation CONSTANT);

  // Syntax highlighting - LSP Semantic Tokens
  Q_PROPERTY(QColor hlType MEMBER hlType CONSTANT);
  Q_PROPERTY(QColor hlFunction MEMBER hlFunction CONSTANT);
  Q_PROPERTY(QColor hlVariable MEMBER hlVariable CONSTANT);
  Q_PROPERTY(QColor hlParameter MEMBER hlParameter CONSTANT);

  // Diagnostic/Comment highlights
  Q_PROPERTY(QColor hlError MEMBER hlError CONSTANT);
  Q_PROPERTY(QColor hlWarning MEMBER hlWarning CONSTANT);
  Q_PROPERTY(QColor hlTodo MEMBER hlTodo CONSTANT);
  Q_PROPERTY(QColor hlInfo MEMBER hlInfo CONSTANT);

  // Markup/Tag highlights
  Q_PROPERTY(QColor hlTag MEMBER hlTag CONSTANT);
  Q_PROPERTY(QColor hlAttribute MEMBER hlAttribute CONSTANT);

  // Module/Import highlights
  Q_PROPERTY(QColor hlModule MEMBER hlModule CONSTANT);

  // Diff highlights
  Q_PROPERTY(QColor hlDiffAdd MEMBER hlDiffAdd CONSTANT);
  Q_PROPERTY(QColor hlDiffDelete MEMBER hlDiffDelete CONSTANT);
  Q_PROPERTY(QColor hlDiffChange MEMBER hlDiffChange CONSTANT);

public:
  explicit Theme(QObject *parent = nullptr);

  QString fontFamily = "JetBrains Mono";
  int fontSizeSmall = 11;
  int fontSizeNormal = 12;
  int fontSizeMedium = 13;
  int fontSizeLarge = 14;

  // Catppuccin Mocha - Base tones
  QColor bgWindow = "#1e1e2e";      // Base
  QColor bgSidebar = "#181825";     // Mantle
  QColor bgHeader = "#181825";      // Mantle
  QColor bgTabBar = "#11111b";      // Crust
  QColor bgTabActive = "#1e1e2e";   // Base
  QColor bgTabHover = "#313244";    // Surface0
  QColor bgEditor = "#1e1e2e";      // Base
  QColor bgGutter = "#181825";      // Mantle
  QColor bgCursorLine = "#313244";  // Surface0

  // Catppuccin Mocha - Surface tones for borders
  QColor borderPrimary = "#45475a";   // Surface1
  QColor borderInactive = "#313244";  // Surface0

  // Catppuccin Mocha - Text tones
  QColor textPrimary = "#cdd6f4";     // Text
  QColor textSecondary = "#bac2de";   // Subtext1
  QColor textMuted = "#a6adc8";       // Subtext0
  QColor textDisabled = "#6c7086";    // Overlay0
  QColor textTree = "#bac2de";        // Subtext1
  QColor textTreeActive = "#cdd6f4";  // Text

  // Catppuccin Mocha - Accent colors
  QColor accentBlue = "#89b4fa";        // Blue
  QColor accentBlueLight = "#b4befe";   // Lavender
  QColor accentBlueDark = "#1e1e2e";    // Base
  QColor accentOrange = "#fab387";      // Peach

  // Catppuccin Mocha - UI elements
  QColor scrollbar = "#585b70";         // Surface2
  QColor selectionBg = "#45475a";       // Surface1
  QColor selectionFg = "#cdd6f4";       // Text
  QColor closeBtnHover = "#313244";     // Surface0
  QColor treeChevron = "#7f849c";       // Overlay1
  QColor treeBullet = "#6c7086";        // Overlay0

  // Catppuccin Mocha - Syntax highlighting (tree-sitter)
  QColor hlKeyword = "#cba6f7";      // Mauve: if, for, return
  QColor hlOperator = "#cdd6f4";    // Text: +, -, =
  QColor hlString = "#a6e3a1";       // Green: "hello"
  QColor hlComment = "#7f849c";     // Overlay1: // comment
  QColor hlNumber = "#fab387";       // Peach: 42
  QColor hlPunctuation = "#9399b2"; // Overlay2: {, }, (, )

  // Catppuccin Mocha - LSP Semantic Tokens
  QColor hlType = "#f9e2af";         // Yellow: MyClass, int
  QColor hlFunction = "#89dceb";      // Sky: myFunc
  QColor hlVariable = "#f2cdcd";      // Flamingo: myVar
  QColor hlParameter = "#f2cdcd";    // Flamingo: param

  // Diagnostic/Comment highlights (Catppuccin Mocha)
  QColor hlError = "#f38ba8";         // Red: error comments
  QColor hlWarning = "#f9e2af";       // Yellow: warning comments
  QColor hlTodo = "#cba6f7";          // Mauve: TODO/FIXME
  QColor hlInfo = "#89b4fa";          // Blue: NOTE/INFO

  // Markup/Tag highlights (Catppuccin Mocha)
  QColor hlTag = "#cba6f7";           // Mauve: HTML/XML tags
  QColor hlAttribute = "#fab387";     // Peach: attributes

  // Module/Import highlights (Catppuccin Mocha)
  QColor hlModule = "#b4befe";        // Lavender: import paths

  // Diff highlights (Catppuccin Mocha)
  QColor hlDiffAdd = "#a6e3a1";       // Green: added lines
  QColor hlDiffDelete = "#f38ba8";    // Red: removed lines
  QColor hlDiffChange = "#f9e2af";    // Yellow: changed lines
};
