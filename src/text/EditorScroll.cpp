#include "EditorScroll.hpp"

#include <QtCore/qpoint.h>

#include "EditorView.hpp"

EditorScroll::EditorScroll(QQuickItem* parent) : QQuickItem(parent) {
  setFlag(ItemClipsChildrenToShape, true);
  setAcceptedMouseButtons(Qt::AllButtons);
}

void EditorScroll::componentComplete() {
  QQuickItem::componentComplete();

  for (auto* child : childItems()) {
    editor_ = qobject_cast<EditorView*>(child);

    if (editor_) {
      break;
    }
  }

  syncView();
}

void EditorScroll::syncView() {
  if (!editor_) {
    return;
  }

  editor_->setWidth(width());
  editor_->setHeight(height());

  editor_->setOffsetX(scrollX_);
  editor_->setOffsetY(scrollY_);

  editor_->update();
}

void EditorScroll::setScrollX(qreal x) {
  if (scrollX_ == x) {
    return;
  }

  scrollX_ = x;

  syncView();

  emit scrollXChanged();
}

void EditorScroll::setScrollY(qreal y) {
  if (scrollY_ == y) {
    return;
  }

  scrollY_ = y;

  syncView();

  emit scrollYChanged();
}

void EditorScroll::wheelEvent(QWheelEvent* event) {
  QPoint delta = event->pixelDelta();

  if (delta.isNull()) delta = event->angleDelta();

  const int dx = delta.x();
  const int dy = delta.y();

  constexpr qreal lockRatio = 1.5;

  if (qAbs(dx) > qAbs(dy) * lockRatio) {
    setScrollX(scrollX_ - dx);
  } else {
    setScrollY(scrollY_ - dy);
  }

  event->accept();
}