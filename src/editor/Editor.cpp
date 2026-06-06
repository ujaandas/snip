#include "Editor.hpp"

#include <QFile>
#include <QTextStream>

#include "Log.hpp"

Editor::Editor(const QString& filePath, QObject* parent)
    : QObject(parent), filePath_(filePath) {}

void Editor::setQuickDocument(QQuickTextDocument* quickDoc) {
  if (!quickDoc) return;
  doc_ = quickDoc->textDocument();
  connect(doc_, &QTextDocument::modificationChanged, this, &Editor::modifiedChanged);
  load(filePath_);
}

bool Editor::isModified() const {
  return doc_ ? doc_->isModified() : false;
}

void Editor::save() {
  if (!saveDocument()) Log::fatal("Failed to save file: {}", filePath_);
}

void Editor::undo() {
  if (doc_) doc_->undo();
}

void Editor::redo() {
  if (doc_) doc_->redo();
}

void Editor::load(const QString& path) {
  if (!doc_) return;

  QFile file(path);
  if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) return;

  QTextStream in(&file);

  doc_->setPlainText(in.readAll());
  doc_->setModified(false);
  filePath_ = path;
}

bool Editor::saveDocument() {
  if (filePath_.isEmpty() || !doc_) return false;

  QFile file(filePath_);
  if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) return false;

  QTextStream out(&file);
  out << doc_->toPlainText();

  doc_->setModified(false);
  return true;
}