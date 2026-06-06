#include "TabManager.hpp"

#include <QFileInfo>

TabManager::TabManager(QObject* parent) : QAbstractListModel(parent) {}

int TabManager::rowCount(const QModelIndex& parent) const {
  if (parent.isValid()) return 0;
  return tabs_.count();
}

QVariant TabManager::data(const QModelIndex& index, int role) const {
  if (!index.isValid() || index.row() >= tabs_.count()) return QVariant();

  const TabData& tab = tabs_.at(index.row());

  if (role == TitleRole) return tab.title;
  if (role == PathRole) return tab.path;
  if (role == EditorRole) return QVariant::fromValue(tab.editor);

  return QVariant();
}

QHash<int, QByteArray> TabManager::roleNames() const {
  QHash<int, QByteArray> roles;
  roles[TitleRole] = "tabTitle";
  roles[PathRole] = "tabPath";
  roles[EditorRole] = "editorController";
  return roles;
}

void TabManager::openTab(const QString& title, const QString& path) {
  for (int i = 0; i < tabs_.count(); ++i) {
    if (tabs_[i].path == path) {
      return;
    }
  }

  EditorController* newEditor = new EditorController(path, this);

  beginInsertRows(QModelIndex(), tabs_.count(), tabs_.count());
  tabs_.append({title, path, newEditor});
  endInsertRows();
}

void TabManager::closeTab(int index) {
  if (index < 0 || index >= tabs_.count()) return;

  beginRemoveRows(QModelIndex(), index, index);

  tabs_[index].editor->deleteLater();
  tabs_.removeAt(index);

  endRemoveRows();
}