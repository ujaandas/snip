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

 public:
  explicit Gutter(QObject* parent = nullptr);

  QQuickTextDocument* textDocument() const { return textDocument_; }
  void setTextDocument(QQuickTextDocument* doc);

  int lineCount() const;

  Q_INVOKABLE qreal lineYPosition(int lineNumber) const;
  Q_INVOKABLE qreal lineHeight(int lineNumber) const;

 signals:
  void textDocumentChanged();
  void lineCountChanged();
  void documentLayoutChanged();

 private:
  void connectDocumentSignals();

  QPointer<QQuickTextDocument> textDocument_;
};
