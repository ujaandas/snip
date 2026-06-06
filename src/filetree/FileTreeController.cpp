#include "FileTreeController.hpp"

#include <QDir>

#include "FileTreeModel.hpp"

FileTreeController::FileTreeController(QObject* parent) : QObject(parent) {
  setRootPath(QDir::currentPath());
}

QObject* FileTreeController::model() const {
  return const_cast<FileTreeModel*>(&model_);
}

QString FileTreeController::rootPath() const { return rootPath_; }
QModelIndex FileTreeController::rootIndex() const { return rootIndex_; }

void FileTreeController::setRootPath(const QString& path) {
  if (path == rootPath_) return;

  rootPath_ = path;

  rootIndex_ = model_.setRootPath(path);

  emit rootPathChanged();
  emit rootIndexChanged();
}