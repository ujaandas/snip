#include "EditorModel.hpp"

#include <QFile>
#include <QTextStream>

EditorModel::EditorModel(QObject* parent) : QObject(parent) {}

QString EditorModel::text() const { return text_; }

void EditorModel::setText(const QString& t) {
  if (text_ == t) return;

  text_ = t;
  emit textChanged();
}

void EditorModel::load(const QString& path) {
  QFile file(path);
  if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
    QTextStream in(&file);
    text_ = in.readAll();
    filePath_ = path;
  }
}

QString EditorModel::filePath() const { return filePath_; }