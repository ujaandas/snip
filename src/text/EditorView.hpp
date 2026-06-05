#pragma once

#include <QFontMetrics>
#include <QQuickPaintedItem>

#include "FileBuffer.hpp"

class EditorView : public QQuickPaintedItem {
  Q_OBJECT;

 private:
  FileBuffer buf_;
  QFont font_;
  QFontMetrics fm_;
  int lineHeight_;
  const int leftMargin_ = 24;

 public:
  explicit EditorView(QQuickItem* parent = nullptr);
  void paint(QPainter* painter) override;
};