#pragma once

#include <QQuickPaintedItem>

#include "FileBuffer.hpp"

class EditorView : public QQuickPaintedItem {
  Q_OBJECT

 private:
  FileBuffer buf_;

 public:
  explicit EditorView(QQuickItem* parent = nullptr);

  void paint(QPainter* painter) override;
};