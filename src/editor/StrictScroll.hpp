#pragma once

#include <QQuickItem>

class StrictScroll : public QQuickItem {
  Q_OBJECT;

  Q_PROPERTY(
      QQuickItem* content READ content WRITE setContent NOTIFY contentChanged);

  Q_PROPERTY(qreal scrollX READ scrollX WRITE setScrollX NOTIFY scrollXChanged);
  Q_PROPERTY(qreal scrollY READ scrollY WRITE setScrollY NOTIFY scrollYChanged);

  Q_PROPERTY(qreal contentWidth READ contentWidth NOTIFY contentWidthChanged);
  Q_PROPERTY(
      qreal contentHeight READ contentHeight NOTIFY contentHeightChanged);

 public:
  explicit StrictScroll(QQuickItem* parent = nullptr);

  QQuickItem* content() const { return content_; }
  void setContent(QQuickItem* item);

  qreal scrollX() const { return scrollX_; }
  qreal scrollY() const { return scrollY_; }

  void setScrollX(qreal x);
  void setScrollY(qreal y);

  qreal contentWidth() const;
  qreal contentHeight() const;

 signals:
  void contentChanged();
  void scrollXChanged();
  void scrollYChanged();
  void contentWidthChanged();
  void contentHeightChanged();

 protected:
  void wheelEvent(QWheelEvent* event) override;
  void geometryChange(const QRectF& newGeom, const QRectF& oldGeom) override;

 private:
  void clampAndApply();
  qreal maxScrollX() const;
  qreal maxScrollY() const;

  QPointer<QQuickItem> content_;

  qreal scrollX_ = 0;
  qreal scrollY_ = 0;
};