#pragma once

#include <QtGui/qevent.h>

#include <QFont>
#include <QFontMetrics>
#include <QQuickPaintedItem>

#include "FileBuffer.hpp"

class EditorView : public QQuickPaintedItem {
  Q_OBJECT;

  Q_PROPERTY(qreal contentWidth READ contentWidth NOTIFY contentWidthChanged);
  Q_PROPERTY(
      qreal contentHeight READ contentHeight NOTIFY contentHeightChanged);

 public:
  explicit EditorView(QQuickItem* parent = nullptr);

  void mousePressEvent(QMouseEvent* event) override;
  // void mouseMoveEvent(QMouseEvent* event) override;
  // void mouseReleaseEvent(QMouseEvent* event) override;

  void paint(QPainter* painter) override;

  qreal contentWidth() const;
  qreal contentHeight() const;

 signals:
  void contentWidthChanged();
  void contentHeightChanged();

 private:
  FileBuffer buf_;
  QFont font_;

  int lineHeight_ = 0;
  const int leftMargin_ = 24;
};