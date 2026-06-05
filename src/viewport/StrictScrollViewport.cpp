#include "StrictScrollViewport.hpp"

#include <QWheelEvent>
#include <QtMath>

StrictScrollViewport::StrictScrollViewport(QQuickItem* parent)
    : QQuickItem(parent) {
  setClip(true);
  setAcceptedMouseButtons(Qt::AllButtons);
}

void StrictScrollViewport::setContent(QQuickItem* item) {
  if (content_ == item) return;

  content_ = item;

  if (content_) {
    content_->setParentItem(this);
  }

  clampAndApply();
  emit contentChanged();
}

qreal StrictScrollViewport::contentWidth() const {
  return content_ ? content_->width() : 0.0;
}

qreal StrictScrollViewport::contentHeight() const {
  return content_ ? content_->height() : 0.0;
}

qreal StrictScrollViewport::maxScrollX() const {
  const int widthExtraLenience = 4;
  return qMax(0.0, contentWidth() - (width() / widthExtraLenience));
}

qreal StrictScrollViewport::maxScrollY() const {
  const double heightExtraLenience =
      1.5;  // TODO: more elegant way to get this number?
  return qMax(0.0, contentHeight() - (height() / heightExtraLenience));
}

void StrictScrollViewport::setScrollX(qreal x) {
  scrollX_ = qBound(0.0, x, maxScrollX());
  if (content_) content_->setX(-scrollX_);
  emit scrollXChanged();
}

void StrictScrollViewport::setScrollY(qreal y) {
  scrollY_ = qBound(0.0, y, maxScrollY());
  if (content_) content_->setY(-scrollY_);
  emit scrollYChanged();
}

void StrictScrollViewport::clampAndApply() {
  setScrollX(scrollX_);
  setScrollY(scrollY_);
}

void StrictScrollViewport::geometryChange(const QRectF& newGeom,
                                          const QRectF& oldGeom) {
  QQuickItem::geometryChange(newGeom, oldGeom);
  clampAndApply();
}

void StrictScrollViewport::wheelEvent(QWheelEvent* event) {
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