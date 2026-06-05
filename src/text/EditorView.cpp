#include "EditorView.hpp"

#include <QFont>
#include <QFontMetrics>
#include <QPainter>
#include <QtMinMax>

EditorView::EditorView(QQuickItem* parent)
    : QQuickPaintedItem(parent),
      buf_("README.md"),
      font_(QFont("JetBrains Mono")),
      fm_(font_) {
  font_.setPixelSize(14);
  lineHeight_ = fm_.height();

  setOpaquePainting(true);
}

void EditorView::paint(QPainter* p) {
  p->fillRect(boundingRect(), Qt::white);

  p->setFont(font_);
  p->setPen(Qt::red);

  const int firstLine = qMax(0, int(offsetY_ / lineHeight_));
  const int visibleLines = int(height() / lineHeight_) + 2;
  const int lastLine = qMin(buf_.lineCount(), firstLine + visibleLines);

  int yPos = -(int(offsetY_) % lineHeight_) + lineHeight_;

  for (int i = firstLine; i < lastLine; ++i) {
    p->drawText(leftMargin_ - offsetX_, yPos, QString(buf_.lineAt(i)));
    yPos += lineHeight_;
  }
}

void EditorView::setOffsetX(qreal x) {
  if (offsetX_ == x) return;

  offsetX_ = x;
  update();

  emit offsetXChanged();
}

void EditorView::setOffsetY(qreal y) {
  if (offsetY_ == y) return;

  offsetY_ = y;
  update();

  emit offsetYChanged();
}

// qreal EditorView::documentHeight() const {
//   return buf_.lineCount() * lineHeight_;
// }

// qreal EditorView::documentWidth() const {
//   int maxWidth = 0;

//   for (int i = 0; i < buf_.lineCount(); ++i) {
//     maxWidth = qMax(maxWidth,
//     fm_.horizontalAdvance(QString(buf_.lineAt(i))));
//   }

//   return maxWidth + leftMargin_;
// }