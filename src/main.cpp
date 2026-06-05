#include <QtQml/qqml.h>

#include <QtCore/QUrl>
#include <QtGui/QFontDatabase>
#include <QtGui/QGuiApplication>
#include <QtQml/QQmlApplicationEngine>

#include "EditorView.hpp"
#include "StrictScrollViewport.hpp"

int main(int argc, char* argv[]) {
  QGuiApplication app(argc, argv);

  QFontDatabase::addApplicationFont(":/assets/fonts/JetBrainsMono[wght].ttf");

  QFont f("JetBrains Mono");
  f.setPixelSize(14);
  f.setWeight(QFont::Normal);

  qmlRegisterType<EditorView>("MyEditor", 1, 0, "EditorView");
  qmlRegisterType<StrictScrollViewport>("MyScroll", 1, 0,
                                        "StrictScrollViewport");

  QQmlApplicationEngine engine;
  engine.load(QUrl(QStringLiteral("qrc:/Main.qml")));

  if (engine.rootObjects().isEmpty()) {
    return -1;
  }

  return app.exec();
}
