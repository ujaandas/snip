#pragma once

#include "FileSortProxy.hpp"

FileSortProxy::FileSortProxy(QObject* parent) : QSortFilterProxyModel(parent) {
  setSortRole(Qt::DisplayRole);
}

bool FileSortProxy::lessThan(const QModelIndex& left,
                             const QModelIndex& right) const {
  auto* fsModel = qobject_cast<QFileSystemModel*>(sourceModel());

  if (!fsModel) return QSortFilterProxyModel::lessThan(left, right);

  bool leftIsDir = fsModel->isDir(left);
  bool rightIsDir = fsModel->isDir(right);

  // folders first
  if (leftIsDir != rightIsDir) return leftIsDir;

  // then alphabetical (case-insensitive)
  QString l = sourceModel()->data(left, Qt::DisplayRole).toString();
  QString r = sourceModel()->data(right, Qt::DisplayRole).toString();

  return QString::compare(l, r, Qt::CaseInsensitive) < 0;
}
