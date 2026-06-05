#include "Editor.hpp"

#include <QFile>
#include <QTextStream>

Editor::Editor(QObject* parent) : QObject(parent) {
  QFile file("README.md");
  if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
    QTextStream in(&file);
    m_text = in.readAll();
  }
}

QString Editor::text() const { return m_text; }

void Editor::setText(const QString& t) {
  if (m_text == t) return;
  m_text = t;
  emit textChanged();
}
