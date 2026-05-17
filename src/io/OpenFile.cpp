#include "OpenFile.h"

#include <qdebug.h>

OpenFile::OpenFile(QString path) : file_(path) {
  if (!file_.open(QIODevice::ReadOnly)) {
    qFatal() << "File could not be opened!";
  }
}

QString OpenFile::read() {
  QString res;
  QTextStream in(&file_);

  while (!in.atEnd()) {
    QString line = in.readLine();
    res += line;
  }

  return res;
}

OpenFile::~OpenFile() { file_.close(); }