#include "EditorController.hpp"

#include <QFile>
#include <QTextStream>

EditorController::EditorController(QObject* parent)
    : QObject(parent), model_(this) {
  QFile file("README.md");
  if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
    QTextStream in(&file);
    model_.setText(in.readAll());
  }

  connect(&model_, &EditorModel::textChanged, this,
          &EditorController::textChanged);
}

QString EditorController::text() const { return model_.text(); }

void EditorController::setText(const QString& t) { model_.setText(t); }