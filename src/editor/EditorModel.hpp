#pragma once

#include <QObject>
#include <QString>
#include <QTextDocument>

class EditorModel : public QObject {
  Q_OBJECT;

 public:
  explicit EditorModel(QObject* parent = nullptr);

  QTextDocument* document();

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