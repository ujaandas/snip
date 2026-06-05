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

void Editor::setText(const QString& t) { m_text = t; }
QString Editor::text() const { return m_text; }