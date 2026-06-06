#include "FileTreeController.hpp"

#include <QDir>

#include "FileSortProxy.hpp"
#include "FileTreeModel.hpp"

FileTreeController::FileTreeController(QObject* parent) : QObject(parent) {
  proxy_.setSourceModel(&model_);
  proxy_.setDynamicSortFilter(true);
  proxy_.sort(0);

  setRootPath(QDir::currentPath());
}

QObject* FileTreeController::model() { return &proxy_; }

QString FileTreeController::rootPath() const { return rootPath_; }
QModelIndex FileTreeController::rootIndex() const { return rootIndex_; }

void FileTreeController::setRootPath(const QString& path) {
  if (path == rootPath_) return;

  rootPath_ = path;
  QModelIndex sourceIndex = model_.setRootPath(path);
  rootIndex_ = proxy_.mapFromSource(sourceIndex);

  emit rootPathChanged();
  emit rootIndexChanged();
}