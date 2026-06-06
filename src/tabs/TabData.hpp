#pragma once

#include <QString>

#include "Editor.hpp"

struct TabData {
  QString title;
  QString path;
  Editor* editor;
};

enum TabRoles { TitleRole = Qt::UserRole + 1, PathRole, EditorRole, ModifiedRole };
