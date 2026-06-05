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

  const qreal scrollY = -y();
  const qreal scrollX = -x();

  const int firstLine = qMax(0, int(scrollY / lineHeight_));
  const int visibleLines = int(height() / lineHeight_) + 2;
  const int lastLine = qMin(buf_.lineCount(), firstLine + visibleLines);

  int yPos = -(int(scrollY) % lineHeight_) + lineHeight_;

  for (int i = firstLine; i < lastLine; ++i) {
    p->drawText(leftMargin_ - scrollX, yPos, QString(buf_.lineAt(i)));
    yPos += lineHeight_;
  }
}

qreal EditorView::documentHeight() const {
  return buf_.lineCount() * lineHeight_;
}

qreal EditorView::documentWidth() const {
  int maxWidth = 0;

  for (int i = 0; i < buf_.lineCount(); ++i) {
    maxWidth = qMax(maxWidth, fm_.horizontalAdvance(QString(buf_.lineAt(i))));
  }

  return maxWidth + leftMargin_;
}