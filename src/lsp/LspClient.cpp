#include "LspClient.hpp"

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QCoreApplication>
#include "Log.hpp"

LspClient::LspClient(QObject* parent) : QObject(parent) {}

LspClient::~LspClient() {
  stop();
}

void LspClient::start(const QString& workspacePath) {
  if (process_) return;

  process_ = new QProcess(this);
  process_->setProgram("clangd");
  process_->setArguments({"--background-index", "--clang-tidy=false"});

  connect(process_, &QProcess::readyReadStandardOutput, this, &LspClient::onReadyRead);
  connect(process_, &QProcess::readyReadStandardError, this, &LspClient::onReadyRead);
  connect(process_, QOverload<QProcess::ProcessError>::of(&QProcess::errorOccurred),
          this, &LspClient::onProcessError);
  connect(process_, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
          this, [this]() { emit serverStopped(); });

  process_->start();
  if (!process_->waitForStarted(5000)) {
    emit serverError("Failed to start clangd");
    return;
  }

  // build initialize params inline
  QJsonObject params;
  params["processId"] = QCoreApplication::applicationPid();
  params["rootUri"] = QUrl::fromLocalFile(workspacePath).toString();
  params["workspaceFolders"] = QJsonArray();

  QJsonObject textDoc;
  textDoc["semanticTokens"] = JsonRpc::semanticTokenCapabilities();
  params["capabilities"] = QJsonObject{{"textDocument", textDoc}};

  sendMessage(JsonRpc::request("initialize", params, requestId_));
}

void LspClient::stop() {
  if (!process_) return;

  if (process_->state() == QProcess::Running) {
    sendMessage(JsonRpc::notification("shutdown"));
    sendMessage(JsonRpc::notification("exit"));
    process_->waitForFinished(2000);
  }

  process_->terminate();
  process_->deleteLater();
  process_ = nullptr;
  initialized_ = false;
}

void LspClient::didOpen(const QString& uri, const QString& languageId, const QString& text) {
  if (!initialized_) return;
  auto params = QJsonObject{{"textDocument", JsonRpc::textDocumentItem(uri, languageId, text)}};
  sendMessage(JsonRpc::notification("textDocument/didOpen", params));
}

void LspClient::didChange(const QString& uri, const QString& text) {
  if (!initialized_) return;
  auto params = QJsonObject{
    {"textDocument", JsonRpc::versionedTextDocument(uri, 2)},
    {"contentChanges", QJsonArray({JsonRpc::fullContentChange(text)})}
  };
  sendMessage(JsonRpc::notification("textDocument/didChange", params));
}

void LspClient::didClose(const QString& uri) {
  if (!initialized_) return;
  auto params = QJsonObject{{"textDocument", JsonRpc::textDocument(uri)}};
  sendMessage(JsonRpc::notification("textDocument/didClose", params));
}

void LspClient::requestSemanticTokens(const QString& uri) {
  if (!initialized_) return;
  int id = requestId_;
  pendingSemanticTokenRequests_[id] = uri;
  auto params = QJsonObject{{"textDocument", JsonRpc::textDocument(uri)}};
  sendMessage(JsonRpc::request("textDocument/semanticTokens/full", params, requestId_));
}

void LspClient::sendMessage(const QJsonObject& message) {
  if (!process_ || process_->state() != QProcess::Running) return;

  QJsonDocument doc(message);
  QByteArray data = doc.toJson(QJsonDocument::Compact);

  QByteArray header = QString("Content-Length: %1\r\n\r\n").arg(data.size()).toUtf8();

  process_->write(header);
  process_->write(data);

  // log sent message
  QString method = message["method"].toString();
  int id = message["id"].toInt();
  if (!method.isEmpty()) {
    Log::info("LSP -> {} (id={})", method, id);
  }
}

void LspClient::onReadyRead() {
  if (!process_) return;

  readBuffer_.append(process_->readAllStandardOutput());

  while (true) {
    if (expectedLength_ < 0) {
      int headerEnd = readBuffer_.indexOf("\r\n\r\n");
      if (headerEnd < 0) break;

      QByteArray header = readBuffer_.left(headerEnd);
      readBuffer_.remove(0, headerEnd + 4);

      for (const QByteArray& line : header.split('\n')) {
        if (line.startsWith("Content-Length: ")) {
          expectedLength_ = line.mid(16).trimmed().toInt();
          break;
        }
      }
    }

    if (expectedLength_ < 0 || readBuffer_.size() < expectedLength_) break;

    QByteArray messageData = readBuffer_.left(expectedLength_);
    readBuffer_.remove(0, expectedLength_);
    expectedLength_ = -1;

    handleMessage(messageData);
  }
}

void LspClient::handleMessage(const QByteArray& data) {
  QJsonDocument doc = QJsonDocument::fromJson(data);
  if (!doc.isObject()) return;

  QJsonObject obj = doc.object();

  // log received message
  int id = obj["id"].toInt();
  QString method = obj["method"].toString();
  if (obj.contains("result")) {
    Log::info("LSP <- result (id={})", id);
  } else if (obj.contains("error")) {
    Log::warning("LSP <- error (id={})", id);
  } else if (!method.isEmpty()) {
    Log::info("LSP <- {} (id={})", method, id);
  }

  if (obj.contains("id") && obj.contains("result")) {
    QJsonObject result = obj["result"].toObject();
    if (result.contains("capabilities")) {
      initialized_ = true;
      emit serverStarted();

      sendMessage(JsonRpc::notification("initialized"));
    } else if (result.contains("data")) {
      int responseId = obj["id"].toInt();
      handleSemanticTokensResponse(responseId, result);
    }
  }
}

void LspClient::handleSemanticTokensResponse(int requestId, const QJsonObject& result) {
  QString uri = pendingSemanticTokenRequests_.take(requestId);
  QJsonArray data = result["data"].toArray();
  emit semanticTokensReceived(uri, data);
}

void LspClient::onProcessError(QProcess::ProcessError error) {
  emit serverError(QString("Process error: %1").arg(error));
}
