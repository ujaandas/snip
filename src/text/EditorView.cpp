#include "EditorView.hpp"

#include <QFont>
#include <QFontMetrics>
#include <QPainter>

EditorView::EditorView(QQuickItem* parent)
    : QQuickPaintedItem(parent), buf_("README.md") {
  setOpaquePainting(true);
}

void EditorView::paint(QPainter* p) {
  p->fillRect(boundingRect(), QColor("#1e1f22"));

  QFont font("JetBrains Mono");
  font.setPixelSize(14);

  p->setFont(font);
  p->setPen(Qt::white);

  QFontMetrics fm(font);

  constexpr int leftMargin = 24;

  int y = 40;

  for (int i = 0; i < buf_.lineCount(); ++i) {
    QStringView line = buf_.lineAt(i);

    p->drawText(leftMargin, y, QString(line));

    y += fm.height();
  }
}