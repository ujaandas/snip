#pragma once

#include <QObject>
#include <QString>

class EditorModel : public QObject {
  Q_OBJECT
  Q_PROPERTY(QString text READ text WRITE setText NOTIFY textChanged)

 public:
  explicit EditorModel(QObject* parent = nullptr);

  QString text() const;
  void setText(const QString& t);

  QString filePath() const;
  void load(const QString& path);
  bool save();

 signals:
  void textChanged();

 private:
  QString doc_;
  QString filePath_;
};