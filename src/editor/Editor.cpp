#include "Editor.hpp"

#include <QFile>
#include <QTextBlock>
#include <QTextCharFormat>
#include <QTextCursor>
#include <QTextStream>
#include <QUrl>

#include "Log.hpp"
#include "LspClient.hpp"
#include "SemanticTokens.hpp"
#include "TreeSitter.hpp"

Editor::Editor(const QString& filePath, QObject* parent)
    : QObject(parent), filePath_(filePath) {}

void Editor::setQuickDocument(QQuickTextDocument* quickDoc) {
  if (!quickDoc) return;
  doc_ = quickDoc->textDocument();
  connect(doc_, &QTextDocument::modificationChanged, this,
          &Editor::modifiedChanged);
  load(filePath_);
  notifyLspDidOpen();
}

bool Editor::isModified() const { return doc_ ? doc_->isModified() : false; }

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

  // apply tree-sitter syntax highlighting
  if (treeSitter_) {
    treeSitter_->highlight(doc_);
  }
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
    disconnect(lspClient_, &LspClient::semanticTokensReceived, this,
               &Editor::onSemanticTokens);
  }
  lspClient_ = lspClient;
  if (lspClient_) {
    connect(lspClient_, &LspClient::semanticTokensReceived, this,
            &Editor::onSemanticTokens);
  }
}

void Editor::setTreeSitter(TreeSitter* treeSitter) {
  treeSitter_ = treeSitter;
}

void Editor::notifyLspDidOpen() {
  if (!lspClient_ || filePath_.isEmpty() || !doc_) return;
  QString uri = QUrl::fromLocalFile(filePath_).toString();
  lspClient_->didOpen(uri, "cpp", doc_->toPlainText());
  lspClient_->requestSemanticTokens(uri);
}

void Editor::onSemanticTokens(const QString& uri, const QJsonArray& data) {
  if (!doc_) return;

  auto tokens = SemanticTokens::parse(data);
  Log::info("Parsed {} semantic tokens for {}", tokens.size(), uri);

  // save modification state
  bool wasModified = doc_->isModified();

  int applied = 0;
  int skipped = 0;

  // apply highlighting to each token
  for (const auto& token : tokens) {
    // get block at token line
    QTextBlock block = doc_->findBlockByNumber(token.line);
    if (!block.isValid()) {
      skipped++;
      continue;
    }

    // calculate absolute position in document
    int startPos = block.position() + token.character;
    int endPos = startPos + token.length;

    // debug: log some tokens to see what we're getting
    if (applied < 3) {
      Log::debug(
          "Token {}: line={}, char={}, len={}, blockPos={}, blockLen={}, "
          "startPos={}, endPos={}, docLen={}",
          applied, token.line, token.character, token.length, block.position(),
          block.length(), startPos, endPos, doc_->characterCount());
    }

    // validate positions
    if (startPos < 0 || endPos > doc_->characterCount()) {
      skipped++;
      continue;
    }
    if (token.character < 0 ||
        token.character + token.length > block.length()) {
      skipped++;
      continue;
    }

    // apply formatting
    QTextCursor cursor(doc_);
    cursor.setPosition(startPos);
    cursor.setPosition(endPos, QTextCursor::KeepAnchor);

    QTextCharFormat format;
    format.setForeground(SemanticTokens::typeToColor(token.type));
    cursor.setCharFormat(format);
    applied++;
  }

  Log::info("Applied {} tokens, skipped {}", applied, skipped);

  // restore modification state
  doc_->setModified(wasModified);
}