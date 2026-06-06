#include <QtQml/qqml.h>

#include <QtCore/QUrl>
#include <QtGui/QFontDatabase>
#include <QtGui/QGuiApplication>
#include <QtQml/QQmlApplicationEngine>
#include <QtQml/QQmlContext>

#include "EditorController.hpp"
#include "filetree/FileTreeController.hpp"
#include "tabs/TabManager.hpp"

int main(int argc, char* argv[]) {
  // Initialize app and engine
  QGuiApplication app(argc, argv);
  QQmlApplicationEngine engine;

  // Instantiate controllers
  EditorController editor;
  FileTreeController ft;
  TabManager tabs;

  // Register controllers
  engine.rootContext()->setContextProperty("editor", &editor);
  engine.rootContext()->setContextProperty("fileTree", &ft);
  engine.rootContext()->setContextProperty("tabs", &tabs);

  // Load font
  QFontDatabase::addApplicationFont(":/assets/fonts/JetBrainsMono[wght].ttf");

  // Load main qml
  engine.load(QUrl(QStringLiteral("qrc:/Main.qml")));

  if (engine.rootObjects().isEmpty()) return -1;

  return app.exec();
}