#pragma once

#include <QObject>
#include <QQuickTextDocument>
#include <QTextDocument>

#include "EditorModel.hpp"

class EditorController : public QObject {
  Q_OBJECT;

 public:
  explicit EditorController(const QString& filePath, QObject* parent = nullptr);

  Q_INVOKABLE void setQuickDocument(QQuickTextDocument* quickDoc);

  Q_INVOKABLE void save();
  Q_INVOKABLE void undo();
  Q_INVOKABLE void redo();

 private:
  QString filePath_;
  EditorModel model_;
};