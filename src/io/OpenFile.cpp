#include "OpenFile.h"

#include <qdebug.h>

OpenFile::OpenFile(QString path) : file_(path) {
  if (!file_.open(QIODevice::ReadOnly)) {
    qFatal("File could not be opened: %s", qPrintable(file_.errorString()));
  }
}

QString OpenFile::read() {
  QTextStream in(&file_);
  return in.readAll();
}

OpenFile::~OpenFile() { file_.close(); }