#pragma once

#include <QFileSystemModel>
#include <QSortFilterProxyModel>

class FileSortProxy : public QSortFilterProxyModel {
  Q_OBJECT

 public:
  explicit FileSortProxy(QObject* parent = nullptr);

 protected:
  bool lessThan(const QModelIndex& left, const QModelIndex& right) const;
};