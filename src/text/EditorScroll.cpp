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

qreal EditorScroll::maxScrollX() const {
  if (!editor_) return 0;
  return qMax(0.0, editor_->documentWidth() - width() / 2);
}

qreal EditorScroll::maxScrollY() const {
  if (!editor_) return 0;
  return qMax(0.0, editor_->documentHeight() - height() / 2);
}

void EditorScroll::setScrollX(qreal x) {
  const qreal clamped = std::clamp(x, 0.0, maxScrollX());

  if (scrollX_ == clamped) return;

  scrollX_ = clamped;
  syncView();
  emit scrollXChanged();
}

void EditorScroll::setScrollY(qreal y) {
  const qreal clamped = std::clamp(y, 0.0, maxScrollY());

  if (scrollY_ == clamped) return;

  scrollY_ = clamped;
  syncView();
  emit scrollYChanged();
}

void EditorScroll::resizeEvent(QResizeEvent*) {
  setScrollX(scrollX_);
  setScrollY(scrollY_);
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