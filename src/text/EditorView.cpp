#include "EditorView.hpp"

#include <QPainter>

EditorView::EditorView(QQuickItem* parent)
    : QQuickPaintedItem(parent),
      buf_("README.md"),
      font_("JetBrains Mono"),
      fm_(font_) {
  font_.setPixelSize(14);
  lineHeight_ = fm_.height();

  setOpaquePainting(true);
}

void EditorView::paint(QPainter* p) {
  p->fillRect(boundingRect(), Qt::white);

  p->setFont(font_);
  p->setPen(Qt::red);

  const int visibleLines = int(height() / lineHeight_) + 2;

  for (int i = 0; i < qMin(buf_.lineCount(), visibleLines); ++i) {
    int y = (i + 1) * lineHeight_;
    p->drawText(leftMargin_, y, QString(buf_.lineAt(i)));
  }
}

qreal EditorView::contentWidth() const {
  int maxWidth = 0;

  for (int i = 0; i < buf_.lineCount(); ++i) {
    maxWidth = qMax(maxWidth, fm_.horizontalAdvance(QString(buf_.lineAt(i))));
  }

  return maxWidth + leftMargin_;
}

qreal EditorView::contentHeight() const {
  return buf_.lineCount() * lineHeight_;
}