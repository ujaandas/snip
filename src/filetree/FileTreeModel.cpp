#include "FileTreeModel.hpp"

FileTreeModel::FileTreeModel(QObject* parent) : QFileSystemModel(parent) {
  setFilter(QDir::AllEntries | QDir::NoDotAndDotDot);
  setResolveSymlinks(true);
}