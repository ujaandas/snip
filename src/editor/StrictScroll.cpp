#include "StrictScroll.hpp"

#include <QWheelEvent>
#include <QtMath>

StrictScroll::StrictScroll(QQuickItem* parent)
    : QQuickItem(parent) {
  setClip(true);
  setAcceptedMouseButtons(Qt::AllButtons);
}

void StrictScroll::setContent(QQuickItem* item) {
  if (content_ == item) return;

  content_ = item;

  if (content_) {
    content_->setParentItem(this);
  }

  clampAndApply();
  emit contentChanged();
}

qreal StrictScroll::contentWidth() const {
  return content_ ? content_->width() : 0.0;
}

qreal StrictScroll::contentHeight() const {
  return content_ ? content_->height() : 0.0;
}

qreal StrictScroll::maxScrollX() const {
  return qMax(0.0, contentWidth() - width());
}

qreal StrictScroll::maxScrollY() const {
  return qMax(0.0, contentHeight() - height());
}

void StrictScroll::setScrollX(qreal x) {
  scrollX_ = qBound(0.0, x, maxScrollX());
  if (content_) content_->setX(-scrollX_);
  emit scrollXChanged();
}

void StrictScroll::setScrollY(qreal y) {
  scrollY_ = qBound(0.0, y, maxScrollY());
  if (content_) content_->setY(-scrollY_);
  emit scrollYChanged();
}

void StrictScroll::clampAndApply() {
  setScrollX(scrollX_);
  setScrollY(scrollY_);
}

void StrictScroll::geometryChange(const QRectF& newGeom,
                                          const QRectF& oldGeom) {
  QQuickItem::geometryChange(newGeom, oldGeom);
  clampAndApply();
}

void StrictScroll::wheelEvent(QWheelEvent* event) {
  QPoint pixelDelta = event->pixelDelta();
  QPointF delta;

  if (!pixelDelta.isNull()) {
    delta = pixelDelta;
  } else {
    constexpr qreal linesPerNotch = 3.0;
    constexpr qreal pixelsPerLine = 20.0;
    constexpr qreal unitsPerNotch = 120.0;
    const qreal scale = linesPerNotch * pixelsPerLine / unitsPerNotch;
    delta = QPointF(event->angleDelta()) * scale;
  }

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