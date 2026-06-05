#include "EditorModel.hpp"

EditorModel::EditorModel(QObject* parent) : QObject(parent) {}

QString EditorModel::text() const { return text_; }

void EditorModel::setText(const QString& t) {
  if (text_ == t) return;

  text_ = t;
  emit textChanged();
}