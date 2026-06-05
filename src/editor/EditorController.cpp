#include "EditorController.hpp"

#include <QtGui/qtextdocument.h>

EditorController::EditorController(QObject* parent) : QObject(parent) {
  model_.load("README.md");
}

QTextDocument* EditorController::document() { return model_.document(); }

QString EditorController::text() { return model_.document()->toPlainText(); }

bool EditorController::save() { return model_.save(); }

void EditorController::undo() { model_.undo(); }

void EditorController::redo() { model_.redo(); }