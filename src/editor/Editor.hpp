#pragma once

#include <QObject>
#include <QQuickTextDocument>
#include <QTextDocument>
#include <QString>

class Editor : public QObject {
  Q_OBJECT;

  Q_PROPERTY(bool isModified READ isModified NOTIFY modifiedChanged);

 public:
  explicit Editor(const QString& filePath, QObject* parent = nullptr);

  Q_INVOKABLE void setQuickDocument(QQuickTextDocument* quickDoc);

  Q_INVOKABLE void save();
  Q_INVOKABLE void undo();
  Q_INVOKABLE void redo();

  bool isModified() const;

 signals:
  void modifiedChanged();

 private:
  void load(const QString& path);
  bool saveDocument();

  QTextDocument* doc_ = nullptr;
  QString filePath_;
};