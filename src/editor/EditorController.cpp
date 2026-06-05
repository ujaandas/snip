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

bool EditorController::save() { return model_.save(); }
void EditorController::undo() { model_.undo(); }
void EditorController::redo() { model_.redo(); }
