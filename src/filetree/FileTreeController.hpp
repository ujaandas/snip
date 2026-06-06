#pragma once

#include <QtCore/qabstractitemmodel.h>

#include <QObject>
#include <QString>

#include "FileTreeModel.hpp"

class FileTreeController : public QObject {
  Q_OBJECT;

  Q_PROPERTY(QObject* model READ model CONSTANT);
  Q_PROPERTY(
      QString rootPath READ rootPath WRITE setRootPath NOTIFY rootPathChanged);
  Q_PROPERTY(QModelIndex rootIndex READ rootIndex NOTIFY rootIndexChanged);

 public:
  explicit FileTreeController(QObject* parent = nullptr);

  QObject* model() const;

  QString rootPath() const;
  void setRootPath(const QString& path);

  QModelIndex rootIndex() const;
  void setRootIndex(const QModelIndex index);

 signals:
  void rootPathChanged();
  void rootIndexChanged();

 private:
  FileTreeModel model_;
  QString rootPath_;
  QModelIndex rootIndex_;
};