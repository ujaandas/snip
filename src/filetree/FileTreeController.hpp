#pragma once

#include <QObject>
#include <QString>

#include "FileTreeModel.hpp"

class FileTreeController : public QObject {
  Q_OBJECT;

  Q_PROPERTY(
      QString rootPath READ rootPath WRITE setRootPath NOTIFY rootPathChanged);

 public:
  explicit FileTreeController(QObject* parent = nullptr);

  QString rootPath() const;
  void setRootPath(const QString& path);

 signals:
  void rootPathChanged();

 private:
  FileTreeModel model_;
  QString rootPath_;
};