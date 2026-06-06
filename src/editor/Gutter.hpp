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
  Q_PROPERTY(int cursorPosition READ cursorPosition WRITE setCursorPosition NOTIFY cursorLineChanged);

 public:
  explicit Gutter(QObject* parent = nullptr);

  QQuickTextDocument* textDocument() const { return textDocument_; }
  void setTextDocument(QQuickTextDocument* doc);

  int lineCount() const;
  int digitCount() const;

  int cursorPosition() const { return cursorPosition_; }
  void setCursorPosition(int position);

  int cursorLine() const { return cursorLine_; }

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
  int cursorPosition_ = -1;
  int cursorLine_ = -1;
};
