#include "EditorController.hpp"

#include <QtGui/qtextdocument.h>

#include "Log.hpp"

EditorController::EditorController(QObject* parent) : QObject(parent) {}

void EditorController::setQuickDocument(QQuickTextDocument* quickDoc) {
  if (!quickDoc) return;
  model_.setDocument(quickDoc->textDocument());
  model_.load("README.md");
}

void EditorController::save() {
  if (!model_.save()) Log::fatal("Failed to save file: {}", model_.filePath());
}

void EditorController::undo() { model_.undo(); }

void EditorController::redo() { model_.redo(); }