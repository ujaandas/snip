#pragma once

#include <QObject>
#include <QQmlEngine>
#include <QQuickTextDocument>
#include <QTextDocument>
#include <QTextBlock>

class Gutter : public QObject {
  Q_OBJECT;
  QML_ELEMENT;

  Q_PROPERTY(
      QQuickTextDocument* textDocument READ textDocument WRITE setTextDocument
          NOTIFY textDocumentChanged);
  Q_PROPERTY(int lineCount READ lineCount NOTIFY lineCountChanged);
  Q_PROPERTY(int digitCount READ digitCount NOTIFY lineCountChanged);
  Q_PROPERTY(int cursorLine READ cursorLine WRITE setCursorLine NOTIFY cursorLineChanged);

 public:
  explicit Gutter(QObject* parent = nullptr);

  QQuickTextDocument* textDocument() const { return textDocument_; }
  void setTextDocument(QQuickTextDocument* doc);

  int lineCount() const;
  int digitCount() const;

  int cursorLine() const { return cursorLine_; }
  void setCursorLine(int line);

  Q_INVOKABLE qreal lineYPosition(int lineNumber) const;
  Q_INVOKABLE qreal lineHeight(int lineNumber) const;

 signals:
  void textDocumentChanged();
  void lineCountChanged();
  void documentLayoutChanged();
  void cursorLineChanged();

 private:
  void connectDocumentSignals();

  QPointer<QQuickTextDocument> textDocument_;
  int cursorLine_ = -1;
};
