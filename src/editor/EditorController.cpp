#include "EditorController.hpp"

#include <QFile>
#include <QTextStream>

EditorController::EditorController(QObject* parent)
    : QObject(parent), model_(this) {
  model_.load("README.md");

  connect(&model_, &EditorModel::textChanged, this,
          &EditorController::textChanged);
}

QString EditorController::text() const { return model_.text(); }

void EditorController::setText(const QString& t) { model_.setText(t); }

bool EditorController::save() {
  const auto path = model_.filePath();

  if (path.isEmpty()) return false;

  QFile file(path);

  if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) return false;

  QTextStream out(&file);
  out << model_.text();

  return true;
}