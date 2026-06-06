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

  Q_PROPERTY(qreal vThumbY READ vThumbY NOTIFY vThumbChanged);
  Q_PROPERTY(qreal vThumbHeight READ vThumbHeight NOTIFY vThumbChanged);
  Q_PROPERTY(bool vThumbNeeded READ vThumbNeeded NOTIFY vThumbChanged);

  Q_PROPERTY(qreal hThumbX READ hThumbX NOTIFY hThumbChanged);
  Q_PROPERTY(qreal hThumbWidth READ hThumbWidth NOTIFY hThumbChanged);
  Q_PROPERTY(bool hThumbNeeded READ hThumbNeeded NOTIFY hThumbChanged);

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

  qreal vThumbY() const;
  qreal vThumbHeight() const;
  bool vThumbNeeded() const;

  qreal hThumbX() const;
  qreal hThumbWidth() const;
  bool hThumbNeeded() const;

  Q_INVOKABLE void dragScrollY(qreal startScrollY, qreal dragDelta, qreal thumbHeight);
  Q_INVOKABLE void dragScrollX(qreal startScrollX, qreal dragDelta, qreal thumbWidth);

 signals:
  void contentChanged();
  void scrollXChanged();
  void scrollYChanged();
  void contentWidthChanged();
  void contentHeightChanged();
  void vThumbChanged();
  void hThumbChanged();

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