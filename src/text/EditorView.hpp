#pragma once

#include <QFontMetrics>
#include <QQuickPaintedItem>

#include "FileBuffer.hpp"

class EditorView : public QQuickPaintedItem {
  Q_OBJECT;

  Q_PROPERTY(qreal offsetX READ offsetX WRITE setOffsetX NOTIFY offsetXChanged);
  Q_PROPERTY(qreal offsetY READ offsetY WRITE setOffsetY NOTIFY offsetYChanged);

 private:
  FileBuffer buf_;
  QFont font_;
  QFontMetrics fm_;
  qreal offsetX_ = 0;
  qreal offsetY_ = 0;
  int lineHeight_ = 0;
  const int leftMargin_ = 24;

 public:
  explicit EditorView(QQuickItem* parent = nullptr);
  void paint(QPainter* painter) override;

  void setOffsetX(qreal x);
  void setOffsetY(qreal y);

  qreal offsetX() const { return offsetX_; }
  qreal offsetY() const { return offsetY_; }

 signals:
  void offsetXChanged();
  void offsetYChanged();
};