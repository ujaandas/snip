#pragma once

#include <QString>

struct TabData {
  QString title;
  QString path;
};

enum TabRoles { TitleRole = Qt::UserRole + 1, PathRole };