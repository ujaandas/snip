#include "EditorView.hpp"

#include <QtGui/qevent.h>

#include <QPainter>

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

void EditorView::focusInEvent(QFocusEvent*) {
  hasFocus_ = true;
  update();
}

void EditorView::focusOutEvent(QFocusEvent*) {
  hasFocus_ = false;
  update();
}

void EditorView::mousePressEvent(QMouseEvent* event) {
  setFocus(true);

  QFontMetrics fm(font_);

  const qreal x = event->position().x() - leftMargin_;
  const qreal y = event->position().y();

  int line = int(y / lineHeight_);
  line = qBound(0, line, buf_.lineCount() - 1);

  const QString& text = QString(buf_.lineAt(line));

  int col = 0;
  int currentX = 0;

  for (int i = 0; i < text.size(); ++i) {
    int charWidth = fm.horizontalAdvance(text[i]);

    if (x < currentX + charWidth / 2.0) {
      col = i;
      break;
    }

    currentX += charWidth;
    col = i + 1;
  }

  cursorLine_ = line;
  cursorCol_ = col;

  update();
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

  // Draw cursor
  if (hasFocus_) {
    QFontMetrics fm(font_);

    int charWidth = fm.horizontalAdvance(' ');
    int x = leftMargin_;
    for (int i = 0; i < cursorCol_; ++i) {
      x += fm.horizontalAdvance(buf_.lineAt(cursorLine_)[i]);
    }

    int y = (cursorLine_ + 1) * lineHeight_;

    p->setPen(Qt::black);
    p->drawLine(x, y - lineHeight_, x, y);
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