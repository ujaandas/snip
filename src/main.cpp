#include <QtQml/qqml.h>

#include <QtCore/QUrl>
#include <QtGui/QFontDatabase>
#include <QtGui/QGuiApplication>
#include <QtQml/QQmlApplicationEngine>
#include <QtQml/QQmlContext>

#include "Editor.hpp"

int main(int argc, char* argv[]) {
  // Initialize app and engine
  QGuiApplication app(argc, argv);
  QQmlApplicationEngine engine;

  // Instantiate controllers
  Editor editor;

  // Register controllers
  engine.rootContext()->setContextProperty("editor", &editor);

  // Load font
  QFontDatabase::addApplicationFont(":/assets/fonts/JetBrainsMono[wght].ttf");

  // Load main qml
  engine.load(QUrl(QStringLiteral("qrc:/Main.qml")));

  if (engine.rootObjects().isEmpty()) return -1;

  return app.exec();
}