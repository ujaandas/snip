#include "FileBuffer.hpp"

#include <qtypes.h>

#include <QFile>
#include <QTextStream>

#include "Log.hpp"

FileBuffer::FileBuffer(QString path) : path_(path) {
  QFile file(path);

  if (!file.open(QIODevice::ReadOnly)) {
    // qFatal("File could not be opened: %s", qPrintable());
    Log::fatal("File '{}' could not be opened: {}", file.fileName(),
               file.errorString());
  }

  QTextStream in(&file);

  while (!in.atEnd()) {
    QString line = in.readLine();
    text_.push_back(line);
  }

  file.close();
}

QStringView FileBuffer::lineAt(qsizetype line) const {
  if (line >= text_.size()) {
    Log::fatal("Requested line exceeds file buffer by {} lines",
               line - text_.size());
  }

  return text_[line];
}

qsizetype FileBuffer::lineCount() const { return text_.size(); }