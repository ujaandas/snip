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
  if (role == ModifiedRole) return tab.editor ? tab.editor->isModified() : false;

  return QVariant();
}

QHash<int, QByteArray> TabManager::roleNames() const {
  QHash<int, QByteArray> roles;
  roles[TitleRole] = "tabTitle";
  roles[PathRole] = "tabPath";
  roles[EditorRole] = "editor";
  roles[ModifiedRole] = "tabModified";
  return roles;
}

void TabManager::setActiveTab(int index) {
  if (index < 0 || index >= tabs_.count()) return;
  if (activeTab_ == index) return;

  activeTab_ = index;
  emit activeTabChanged();
}

int TabManager::activeTab() const { return activeTab_; }

Editor* TabManager::activeEditor() const {
  if (activeTab_ < 0 || activeTab_ >= tabs_.count()) return nullptr;
  return tabs_[activeTab_].editor;
}

void TabManager::openTab(const QString& title, const QString& path) {
  for (int i = 0; i < tabs_.count(); ++i) {
    if (tabs_[i].path == path) {
      setActiveTab(i);
      return;
    }
  }

  Editor* newEditor = new Editor(path, this);
  connect(newEditor, &Editor::modifiedChanged, this, &TabManager::onEditorModifiedChanged);

  beginInsertRows(QModelIndex(), tabs_.count(), tabs_.count());
  tabs_.append({title, path, newEditor});
  endInsertRows();

  setActiveTab(tabs_.count() - 1);
}

void TabManager::closeTab(int index) {
  if (index < 0 || index >= tabs_.count()) return;

  const int previousActiveTab = activeTab_;
  beginRemoveRows(QModelIndex(), index, index);
  tabs_[index].editor->deleteLater();
  tabs_.removeAt(index);
  endRemoveRows();

  if (tabs_.isEmpty()) {
    activeTab_ = -1;
    emit activeTabChanged();
  } else if (previousActiveTab == index) {
    setActiveTab(qMin(index, tabs_.count() - 1));
  } else if (previousActiveTab > index) {
    setActiveTab(previousActiveTab - 1);
  } else if (activeTab_ >= tabs_.count()) {
    setActiveTab(tabs_.count() - 1);
  } else {
    emit activeTabChanged();
  }
}

void TabManager::closeActiveTab() {
  closeTab(activeTab_);
}

void TabManager::nextTab() {
  if (tabs_.isEmpty()) return;
  setActiveTab((activeTab_ + 1) % tabs_.count());
}

void TabManager::prevTab() {
  if (tabs_.isEmpty()) return;
  setActiveTab((activeTab_ - 1 + tabs_.count()) % tabs_.count());
}

void TabManager::onEditorModifiedChanged() {
  auto* editor = qobject_cast<Editor*>(sender());
  if (!editor) return;
  for (int i = 0; i < tabs_.count(); ++i) {
    if (tabs_[i].editor == editor) {
      const QModelIndex idx = index(i);
      emit dataChanged(idx, idx, {ModifiedRole});
      return;
    }
  }
}