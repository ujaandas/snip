#include <QQuickItem>

#include "EditorView.hpp"

class EditorScroll : public QQuickItem {
  Q_OBJECT;

  Q_PROPERTY(qreal scrollX READ scrollX NOTIFY scrollXChanged);
  Q_PROPERTY(qreal scrollY READ scrollY NOTIFY scrollYChanged);

 public:
  explicit EditorScroll(QQuickItem* parent = nullptr);

  qreal scrollX() const { return scrollX_; }
  qreal scrollY() const { return scrollY_; }

  void setScrollX(qreal x);
  void setScrollY(qreal y);

 signals:
  void scrollXChanged();
  void scrollYChanged();

 protected:
  void wheelEvent(QWheelEvent* event) override;
  void componentComplete() override;

 private:
  void syncView();

  qreal scrollX_ = 0;
  qreal scrollY_ = 0;

  QPointer<EditorView> editor_;
};