#pragma once

#include <QObject>
#include <QString>
#include <QTextDocument>

class EditorModel : public QObject {
  Q_OBJECT;
  Q_PROPERTY(QString text READ text WRITE setText NOTIFY textChanged);

 public:
  explicit EditorModel(QObject* parent = nullptr);

  QString text() const;
  void setText(const QString& t);

  QString filePath() const;

  void load(const QString& path);
  bool save();

  void undo();
  void redo();

 signals:
  void textChanged();

 private:
  QTextDocument doc_;
  QString filePath_;
};