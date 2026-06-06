#pragma once

#include <QObject>
#include <QQuickTextDocument>
#include <QTextDocument>
#include <QString>
#include <QJsonArray>

class LspClient;

class Editor : public QObject {
  Q_OBJECT;

  Q_PROPERTY(bool isModified READ isModified NOTIFY modifiedChanged);

 public:
  explicit Editor(const QString& filePath, QObject* parent = nullptr);

  Q_INVOKABLE void setQuickDocument(QQuickTextDocument* quickDoc);
  void setLspClient(LspClient* lspClient);

  Q_INVOKABLE void save();
  Q_INVOKABLE void undo();
  Q_INVOKABLE void redo();

  bool isModified() const;

 signals:
  void modifiedChanged();

 private slots:
  void onSemanticTokens(const QString& uri, const QJsonArray& data);

 private:
  void load(const QString& path);
  bool saveDocument();
  void notifyLspDidOpen();

  QTextDocument* doc_ = nullptr;
  QString filePath_;
  LspClient* lspClient_ = nullptr;
};