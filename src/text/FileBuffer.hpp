#pragma once

#include <QString>
#include <QVector>
#include <QtTypes>

class FileBuffer {
 private:
  QVector<QString> text_;
  QString path_;

 public:
  explicit FileBuffer(QString path);

  QStringView lineAt(qsizetype line) const;
  qsizetype lineCount() const;
};