#include "EditorScroll.hpp"

#include <QWheelEvent>
#include <QtMath>

#include "EditorScroll.hpp"

EditorScroll::EditorScroll(QQuickItem* parent) : QQuickItem(parent) {
  setClip(true);
  setAcceptedMouseButtons(Qt::AllButtons);
}

void EditorScroll::setContent(QQuickItem* item) {
  if (content_ == item) return;

  content_ = item;

  if (content_) {
    content_->setParentItem(this);
  }

  clampAndApply();
  emit contentChanged();
}

qreal EditorScroll::contentWidth() const {
  return content_ ? content_->width() : 0.0;
}

qreal EditorScroll::contentHeight() const {
  return content_ ? content_->height() : 0.0;
}

qreal EditorScroll::maxScrollX() const {
  return qMax(0.0, contentWidth() - width());
}

qreal EditorScroll::maxScrollY() const {
  return qMax(0.0, contentHeight() - height());
}

void EditorScroll::setScrollX(qreal x) {
  scrollX_ = qBound(0.0, x, maxScrollX());
  if (content_) content_->setX(-scrollX_);
  emit scrollXChanged();
}

void EditorScroll::setScrollY(qreal y) {
  scrollY_ = qBound(0.0, y, maxScrollY());
  if (content_) content_->setY(-scrollY_);
  emit scrollYChanged();
}

void EditorScroll::clampAndApply() {
  setScrollX(scrollX_);
  setScrollY(scrollY_);
}

void EditorScroll::geometryChange(const QRectF& newGeom,
                                  const QRectF& oldGeom) {
  QQuickItem::geometryChange(newGeom, oldGeom);
  clampAndApply();
}

void EditorScroll::wheelEvent(QWheelEvent* event) {
  QPoint delta = event->pixelDelta();
  if (delta.isNull()) delta = event->angleDelta();

  const qreal dx = delta.x();
  const qreal dy = delta.y();

  constexpr qreal lockRatio = 1.5;

  if (qAbs(dx) > qAbs(dy) * lockRatio) {
    setScrollX(scrollX_ - dx);
  } else {
    setScrollY(scrollY_ - dy);
  }

  event->accept();
}