#pragma once

#include <QObject>
#include <QQuickTextDocument>
#include <QString>
#include <QTextDocument>

class EditorController : public QObject {
  Q_OBJECT
  Q_PROPERTY(QString text READ text WRITE setText NOTIFY textChanged)

 public:
  explicit EditorController(QObject* parent = nullptr);

  QString text() const;
  void setText(const QString& t);

 signals:
  void textChanged();

 private:
  QString m_text;
};