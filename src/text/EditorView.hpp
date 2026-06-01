#pragma once

#include <QQuickItem>

#include "FileBuffer.hpp"

class EditorView : public QQuickItem {
  Q_OBJECT

 private:
  FileBuffer buf_;

 protected:
  QSGNode* updatePaintNode(QSGNode* oldNode, UpdatePaintNodeData*) override;

 public:
  explicit EditorView(QQuickItem* parent = nullptr);
};