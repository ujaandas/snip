#pragma once

#include <qobject.h>

#include <QFile>

class OpenFile {
 private:
  QFile file_;

 public:
  explicit OpenFile(QString path);

  QString read();

  OpenFile(const OpenFile&) = delete;
  OpenFile& operator=(const OpenFile&) = delete;

  OpenFile(OpenFile&&) = delete;
  OpenFile& operator=(OpenFile&&) = delete;

  ~OpenFile();
};