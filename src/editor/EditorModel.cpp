#include "EditorModel.hpp"

#include <QFile>
#include <QTextStream>

EditorModel::EditorModel(QObject* parent) : QObject(parent) {}

QString EditorModel::text() const { return doc_; }

void EditorModel::setText(const QString& t) {
  if (doc_ == t) return;

  doc_ = t;
  emit textChanged();
}

void EditorModel::load(const QString& path) {
  QFile file(path);
  if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
    QTextStream in(&file);
    doc_ = in.readAll();
    filePath_ = path;
  }
}

bool EditorModel::save() {
  if (filePath_.isEmpty()) return false;

  QFile file(filePath_);

  if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) return false;

  QTextStream out(&file);
  out << doc_;

  file.close();

  return true;
}

QString EditorModel::filePath() const { return filePath_; }