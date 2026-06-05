#include "EditorView.hpp"

#include <QtGui/qevent.h>

#include <QPainter>

#include "Log.hpp"

EditorView::EditorView(QQuickItem* parent)
    : QQuickPaintedItem(parent), buf_("README.md"), font_("JetBrains Mono") {
  font_.setPixelSize(14);
  lineHeight_ = QFontMetrics(font_).height();

  setAcceptedMouseButtons(Qt::AllButtons);
  setAcceptHoverEvents(true);
  setFlag(ItemIsFocusScope, true);
  setFlag(ItemHasContents, true);

  setOpaquePainting(true);
}

void EditorView::mousePressEvent(QMouseEvent* event) {
  QPointF local = event->position();
  QPointF global = event->globalPosition();

  Log::debug("local: {},{}", local.x(), local.y());
  Log::debug("global: {},{}", global.x(), global.y());
}

void EditorView::paint(QPainter* p) {
  p->fillRect(boundingRect(), Qt::white);

  p->setFont(font_);
  p->setPen(Qt::red);

  const qreal xOffset = -x();
  const qreal yOffset = -y();

  p->translate(xOffset, yOffset);

  const int visibleLines = int(height() / lineHeight_) + 2;

  for (int i = 0; i < qMin(buf_.lineCount(), visibleLines); i++) {
    int y = (i + 1) * lineHeight_;
    p->drawText(leftMargin_, y, QString(buf_.lineAt(i)));
  }
}

qreal EditorView::contentWidth() const {
  int maxWidth = 0;

  QFontMetrics fm(font_);

  for (int i = 0; i < buf_.lineCount(); i++) {
    maxWidth = qMax(maxWidth, fm.horizontalAdvance(QString(buf_.lineAt(i))));
  }

  return maxWidth + leftMargin_;
}

qreal EditorView::contentHeight() const {
  return buf_.lineCount() * lineHeight_;
}