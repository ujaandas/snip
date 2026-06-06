#pragma once

#include <QObject>
#include <QProcess>
#include <QJsonObject>
#include <QJsonArray>

#include "JsonRpc.hpp"

class LspClient : public QObject {
  Q_OBJECT

 public:
  explicit LspClient(QObject* parent = nullptr);
  ~LspClient();

  void start(const QString& workspacePath);
  void stop();

  // File operations
  void didOpen(const QString& uri, const QString& languageId, const QString& text);
  void didChange(const QString& uri, const QString& text);
  void didClose(const QString& uri);

  // Semantic tokens
  void requestSemanticTokens(const QString& uri);

 signals:
  void semanticTokensReceived(const QString& uri, const QJsonArray& data);
  void serverError(const QString& message);
  void serverStarted();
  void serverStopped();

 private slots:
  void onReadyRead();
  void onProcessError(QProcess::ProcessError error);

 private:
  void sendMessage(const QJsonObject& message);
  void handleMessage(const QByteArray& data);
  void handleSemanticTokensResponse(int requestId, const QJsonObject& result);

  QProcess* process_ = nullptr;
  QByteArray readBuffer_;
  int expectedLength_ = -1;
  bool initialized_ = false;
  int requestId_ = 1;
  QHash<int, QString> pendingSemanticTokenRequests_;  // id -> uri
};
