#pragma once

#include <QFileSystemModel>
#include <QObject>
#include <QString>

#include <QtCore/qabstractitemmodel.h>

#include "FileSortProxy.hpp"

class FileTree : public QObject {
  Q_OBJECT;

  Q_PROPERTY(QObject* model READ model CONSTANT);
  Q_PROPERTY(
      QString rootPath READ rootPath WRITE setRootPath NOTIFY rootPathChanged);
  Q_PROPERTY(QModelIndex rootIndex READ rootIndex NOTIFY rootIndexChanged);

 public:
  explicit FileTree(QObject* parent = nullptr);

  QObject* model();

  QString rootPath() const;
  void setRootPath(const QString& path);

  QModelIndex rootIndex() const;

 signals:
  void rootPathChanged();
  void rootIndexChanged();

 private:
  FileSortProxy proxy_;
  QFileSystemModel model_;
  QString rootPath_;
  QModelIndex rootIndex_;
};