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
  emit hThumbChanged();
}

void StrictScroll::setScrollY(qreal y) {
  scrollY_ = qBound(0.0, y, maxScrollY());
  if (content_) content_->setY(-scrollY_);
  emit scrollYChanged();
  emit vThumbChanged();
}

void StrictScroll::clampAndApply() {
  setScrollX(scrollX_);
  setScrollY(scrollY_);
}

void StrictScroll::geometryChange(const QRectF& newGeom,
                                          const QRectF& oldGeom) {
  QQuickItem::geometryChange(newGeom, oldGeom);
  clampAndApply();
  emit vThumbChanged();
  emit hThumbChanged();
}

qreal StrictScroll::vThumbHeight() const {
  const qreal ch = contentHeight();
  if (ch <= 0.0) return height();
  return qMax(24.0, (height() / ch) * (height() - 4.0));
}

qreal StrictScroll::vThumbY() const {
  const qreal track = height() - vThumbHeight();
  const qreal maxS = maxScrollY();
  if (maxS <= 0.0) return 2.0;
  return (scrollY_ / maxS) * track + 2.0;
}

bool StrictScroll::vThumbNeeded() const {
  return contentHeight() > height();
}

qreal StrictScroll::hThumbWidth() const {
  const qreal cw = contentWidth();
  if (cw <= 0.0) return width();
  return qMax(24.0, (width() / cw) * (width() - 4.0));
}

qreal StrictScroll::hThumbX() const {
  const qreal track = width() - hThumbWidth();
  const qreal maxS = maxScrollX();
  if (maxS <= 0.0) return 2.0;
  return (scrollX_ / maxS) * track + 2.0;
}

bool StrictScroll::hThumbNeeded() const {
  return contentWidth() > width();
}

void StrictScroll::dragScrollY(qreal startScrollY, qreal dragDelta, qreal thumbHeight) {
  const qreal track = height() - thumbHeight;
  if (track <= 0.0) return;
  setScrollY(startScrollY + (dragDelta / track) * maxScrollY());
}

void StrictScroll::dragScrollX(qreal startScrollX, qreal dragDelta, qreal thumbWidth) {
  const qreal track = width() - thumbWidth;
  if (track <= 0.0) return;
  setScrollX(startScrollX + (dragDelta / track) * maxScrollX());
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