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

 public:
  explicit Theme(QObject* parent = nullptr);

  QString fontFamily = "JetBrains Mono";
  int fontSizeSmall = 11;
  int fontSizeNormal = 12;
  int fontSizeMedium = 13;
  int fontSizeLarge = 14;

  QColor bgWindow = "#161a22";
  QColor bgSidebar = "#181d27";
  QColor bgHeader = "#161b24";
  QColor bgTabBar = "#171c26";
  QColor bgTabActive = "#1f2430";
  QColor bgTabHover = "#1b202b";
  QColor bgEditor = "#1f2430";
  QColor bgGutter = "#1a1e26";
  QColor bgCursorLine = "#2a3040";

  QColor borderPrimary = "#2b313d";
  QColor borderInactive = "#171c26";

  QColor textPrimary = "#e6ecf8";
  QColor textSecondary = "#a9b2c3";
  QColor textMuted = "#9aa4b5";
  QColor textDisabled = "#4e5668";
  QColor textTree = "#d7deeb";
  QColor textTreeActive = "#e8edf5";

  QColor accentBlue = "#4d9ef5";
  QColor accentBlueLight = "#99c4ff";
  QColor accentBlueDark = "#1e3a5f";
  QColor accentOrange = "#e6bd6a";

  QColor scrollbar = "#4a5268";
  QColor selectionBg = "#214b78";
  QColor selectionFg = "#ffffff";
  QColor closeBtnHover = "#3a4152";
  QColor treeChevron = "#8f99aa";
  QColor treeBullet = "#6e7787";
};
