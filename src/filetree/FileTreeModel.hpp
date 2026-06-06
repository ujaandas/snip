#pragma once

#include <QFileSystemModel>
#include <QObject>

class FileTreeModel : public QFileSystemModel {
  Q_OBJECT

 public:
  explicit FileTreeModel(QObject* parent = nullptr);
};