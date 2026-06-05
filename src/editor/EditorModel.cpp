#include "EditorModel.hpp"

#include <QFile>
#include <QTextStream>

EditorModel::EditorModel(QObject* parent) : QObject(parent) {
  connect(&doc_, &QTextDocument::contentsChanged, this,
          &EditorModel::textChanged);
}

QString EditorModel::text() const { return doc_.toPlainText(); }

void EditorModel::setText(const QString& text) {
  if (doc_.toPlainText() == text) return;

  doc_.setPlainText(text);
}

void EditorModel::load(const QString& path) {
  QFile file(path);

  if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) return;

  QTextStream in(&file);

  doc_.setPlainText(in.readAll());
  filePath_ = path;
}

bool EditorModel::save() {
  if (filePath_.isEmpty()) return false;

  QFile file(filePath_);

  if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) return false;

  QTextStream out(&file);
  out << doc_.toPlainText();

  return true;
}

QString EditorModel::filePath() const { return filePath_; }

void EditorModel::undo() { doc_.undo(); }

void EditorModel::redo() { doc_.redo(); }