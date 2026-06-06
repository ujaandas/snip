#pragma once

#include <QString>

#include "EditorController.hpp"

struct TabData {
  QString title;
  QString path;
  EditorController* editor;
};

enum TabRoles { TitleRole = Qt::UserRole + 1, PathRole, EditorRole };