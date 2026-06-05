#pragma once

#include <QObject>

#include "EditorModel.hpp"

class EditorController : public QObject {
  Q_OBJECT
  Q_PROPERTY(QString text READ text WRITE setText NOTIFY textChanged)

 public:
  explicit EditorController(QObject* parent = nullptr);

  QString text() const;
  void setText(const QString& t);

  Q_INVOKABLE bool save();
  // Q_INVOKABLE bool undo();
  // Q_INVOKABLE bool redo();

 signals:
  void textChanged();

 private:
  EditorModel model_;
};