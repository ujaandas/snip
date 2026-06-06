#include "Editor.hpp"

#include <QFile>
#include <QTextStream>
#include <QUrl>
#include <QTextCursor>
#include <QTextCharFormat>
#include <algorithm>

#include "LspClient.hpp"
#include "Log.hpp"
#include "SemanticTokens.hpp"

Editor::Editor(const QString& filePath, QObject* parent)
    : QObject(parent), filePath_(filePath) {}

void Editor::setQuickDocument(QQuickTextDocument* quickDoc) {
  if (!quickDoc) return;
  doc_ = quickDoc->textDocument();
  connect(doc_, &QTextDocument::modificationChanged, this, &Editor::modifiedChanged);
  load(filePath_);
  notifyLspDidOpen();
}

bool Editor::isModified() const {
  return doc_ ? doc_->isModified() : false;
}

void Editor::save() {
  if (!saveDocument()) Log::fatal("Failed to save file: {}", filePath_);
}

void Editor::undo() {
  if (doc_) doc_->undo();
}

void Editor::redo() {
  if (doc_) doc_->redo();
}

void Editor::load(const QString& path) {
  if (!doc_) return;

  QFile file(path);
  if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) return;

  QTextStream in(&file);

  doc_->setPlainText(in.readAll());
  doc_->setModified(false);
  filePath_ = path;
}

bool Editor::saveDocument() {
  if (filePath_.isEmpty() || !doc_) return false;

  QFile file(filePath_);
  if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) return false;

  QTextStream out(&file);
  out << doc_->toPlainText();

  doc_->setModified(false);
  return true;
}

void Editor::setLspClient(LspClient* lspClient) {
  if (lspClient_) {
    disconnect(lspClient_, &LspClient::semanticTokensReceived, this, &Editor::onSemanticTokens);
  }
  lspClient_ = lspClient;
  if (lspClient_) {
    connect(lspClient_, &LspClient::semanticTokensReceived, this, &Editor::onSemanticTokens);
  }
}

void Editor::notifyLspDidOpen() {
  if (!lspClient_ || filePath_.isEmpty() || !doc_) return;
  QString uri = QUrl::fromLocalFile(filePath_).toString();
  lspClient_->didOpen(uri, "cpp", doc_->toPlainText());
  lspClient_->requestSemanticTokens(uri);
}

void Editor::onSemanticTokens(const QString& uri, const QJsonArray& data) {
  auto tokens = SemanticTokens::parse(data);
  Log::info("Parsed {} semantic tokens for {}", tokens.size(), uri);

  // Log first few tokens for debugging
  for (int i = 0; i < qMin(5, tokens.size()); ++i) {
    const auto& t = tokens[i];
    Log::debug("Token {}: line={}, char={}, len={}, type={}, mod={}",
               i, t.line, t.character, t.length, t.type, t.modifiers);
  }
}