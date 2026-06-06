#include "FileTreeController.hpp"

#include <QDir>

#include "FileTreeModel.hpp"

FileTreeController::FileTreeController(QObject* parent) : QObject(parent) {
  setRootPath(QDir::homePath());
}

QString FileTreeController::rootPath() const { return rootPath_; }

void FileTreeController::setRootPath(const QString& path) {
  if (path == rootPath_) return;

  rootPath_ = path;

  model_.setRootPath(path);

  emit rootPathChanged();
}