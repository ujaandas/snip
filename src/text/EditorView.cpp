#include "EditorView.hpp"

#include <qstringview.h>

#include <QChar>
#include <QQuickItem>
#include <QQuickWindow>
#include <QSGNode>
#include <QSGTextNode>
#include <QTextLayout>

EditorView::EditorView(QQuickItem* parent)
    : QQuickItem(parent), buf_("README.md") {
  setFlag(ItemHasContents, true);
};

QSGNode* EditorView::updatePaintNode(QSGNode* oldNode, UpdatePaintNodeData*) {
  QSGNode* root = oldNode;

  if (!root) root = new QSGNode();

  // clear previous frame
  while (auto* child = root->firstChild()) {
    root->removeChildNode(child);
    delete child;
  }

  QFont font("JetBrains Mono", 12);

  constexpr float lineHeight = 18.0f;
  constexpr float leftMargin = 8.0f;

  const int lineCount = buf_.lineCount();

  for (int i = 0; i < lineCount; i++) {
    QStringView text = buf_.lineAt(i);

    auto* textNode = window()->createTextNode();

    textNode->setColor(Qt::white);
    textNode->setViewport(boundingRect());

    QTextLayout* layout = new QTextLayout(QString(text), font);

    layout->beginLayout();
    QTextLine line = layout->createLine();
    line.setLineWidth(10000);
    line.setPosition(QPointF(0, 0));
    layout->endLayout();

    textNode->addTextLayout(QPointF(leftMargin, i * lineHeight), layout);

    root->appendChildNode(textNode);
  }

  return root;
}