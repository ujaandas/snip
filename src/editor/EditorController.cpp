#include "EditorController.hpp"

#include <QFile>
#include <QTextStream>

EditorController::EditorController(QObject* parent) : QObject(parent) {
  QFile file("README.md");
  if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
    QTextStream in(&file);
    m_text = in.readAll();
  }
}

void EditorController::setText(const QString& t) { m_text = t; }
QString EditorController::text() const { return m_text; }