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

  return QVariant();
}

QHash<int, QByteArray> TabManager::roleNames() const {
  QHash<int, QByteArray> roles;
  roles[TitleRole] = "tabTitle";
  roles[PathRole] = "tabPath";
  return roles;
}

void TabManager::openTab(const QString& title, const QString& path) {
  // Optional: Check if the tab is already open to avoid duplicates
  for (int i = 0; i < tabs_.count(); ++i) {
    if (tabs_[i].path == path) {
      // Here you could emit a signal to tell QML to switch to this tab index
      return;
    }
  }

  beginInsertRows(QModelIndex(), tabs_.count(), tabs_.count());
  tabs_.append({title, path});
  endInsertRows();
}

void TabManager::closeTab(int index) {
  if (index < 0 || index >= tabs_.count()) return;

  beginRemoveRows(QModelIndex(), index, index);
  tabs_.removeAt(index);
  endRemoveRows();
}