#pragma once

#include <QtGui/qtextdocument.h>

#include <QObject>

#include "EditorModel.hpp"

class EditorController : public QObject {
  Q_OBJECT;

 public:
  explicit EditorController(QObject* parent = nullptr);

  Q_INVOKABLE QTextDocument* document();
  Q_INVOKABLE QString text();

  Q_INVOKABLE bool save();
  Q_INVOKABLE void undo();
  Q_INVOKABLE void redo();

 private:
  EditorModel model_;
};