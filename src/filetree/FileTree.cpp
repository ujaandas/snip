#include "FileTree.hpp"

#include <QDir>

FileTree::FileTree(QObject* parent) : QObject(parent) {
  model_.setFilter(QDir::AllEntries | QDir::NoDotAndDotDot);
  model_.setResolveSymlinks(true);

  proxy_.setSourceModel(&model_);
  proxy_.setDynamicSortFilter(true);
  proxy_.sort(0);

  setRootPath(QDir::currentPath());
}

QObject* FileTree::model() { return &proxy_; }

QString FileTree::rootPath() const { return rootPath_; }
QModelIndex FileTree::rootIndex() const { return rootIndex_; }

void FileTree::setRootPath(const QString& path) {
  if (path == rootPath_) return;

  rootPath_ = path;
  QModelIndex sourceIndex = model_.setRootPath(path);
  rootIndex_ = proxy_.mapFromSource(sourceIndex);

  emit rootPathChanged();
  emit rootIndexChanged();
}