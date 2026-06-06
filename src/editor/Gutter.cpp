#include "Gutter.hpp"

#include <QTextLayout>

Gutter::Gutter(QObject* parent)
    : QObject(parent) {}

void Gutter::setTextDocument(QQuickTextDocument* doc) {
  if (textDocument_ == doc) return;

  if (textDocument_ && textDocument_->textDocument()) {
    disconnect(
        textDocument_->textDocument(), &QTextDocument::contentsChanged, this,
        &Gutter::lineCountChanged);
    disconnect(
        textDocument_->textDocument(), &QTextDocument::blockCountChanged, this,
        &Gutter::lineCountChanged);
  }

  textDocument_ = doc;
  connectDocumentSignals();

  emit textDocumentChanged();
  emit lineCountChanged();
}

void Gutter::connectDocumentSignals() {
  if (!textDocument_ || !textDocument_->textDocument()) return;

  auto* doc = textDocument_->textDocument();

  connect(
      doc, &QTextDocument::contentsChanged, this,
      &Gutter::lineCountChanged);
  connect(
      doc, &QTextDocument::blockCountChanged, this,
      &Gutter::lineCountChanged);
  connect(
      doc, &QTextDocument::documentLayoutChanged, this,
      &Gutter::documentLayoutChanged);
}

int Gutter::lineCount() const {
  if (!textDocument_ || !textDocument_->textDocument()) return 0;
  return textDocument_->textDocument()->blockCount();
}

qreal Gutter::lineYPosition(int lineNumber) const {
  if (!textDocument_ || !textDocument_->textDocument()) return 0.0;

  auto* doc = textDocument_->textDocument();
  QTextBlock block = doc->findBlockByNumber(lineNumber);

  if (!block.isValid()) return 0.0;

  QTextLayout* layout = block.layout();
  if (!layout) return 0.0;

  QRectF rect = layout->boundingRect();
  return block.layout()->position().y();
}

qreal Gutter::lineHeight(int lineNumber) const {
  if (!textDocument_ || !textDocument_->textDocument()) return 0.0;

  auto* doc = textDocument_->textDocument();
  QTextBlock block = doc->findBlockByNumber(lineNumber);

  if (!block.isValid()) return 0.0;

  QTextLayout* layout = block.layout();
  if (!layout) return 0.0;

  return layout->boundingRect().height();
}
