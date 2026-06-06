#include <QtQml/qqml.h>

#include <QtCore/QUrl>
#include <QtGui/QFontDatabase>
#include <QtGui/QGuiApplication>
#include <QQuickStyle>
#include <QtQml/QQmlApplicationEngine>
#include <QtQml/QQmlContext>

#include "FileTree.hpp"
#include "editor/StrictScroll.hpp"
#include "tabs/TabManager.hpp"

int main(int argc, char* argv[]) {
  // Use a non-native Quick Controls style so QML control customization is supported.
  QQuickStyle::setStyle("Basic");

  // Initialize app and engine
  QGuiApplication app(argc, argv);
  QQmlApplicationEngine engine;

  FileTree fileTree;
  TabManager tabs;

  qmlRegisterType<StrictScroll>("Snip.Editor", 1, 0, "StrictScroll");

  engine.rootContext()->setContextProperty("fileTree", &fileTree);
  engine.rootContext()->setContextProperty("tabManager", &tabs);

  // Load font
  QFontDatabase::addApplicationFont(":/assets/fonts/JetBrainsMono[wght].ttf");

  // Load main qml
  engine.load(QUrl(QStringLiteral("qrc:/Main.qml")));

  if (engine.rootObjects().isEmpty()) return -1;

  return app.exec();
}