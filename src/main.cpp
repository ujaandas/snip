#include <QtCore/QUrl>
#include <QtGui/QGuiApplication>
#include <QtQml/QQmlApplicationEngine>
#include <iostream>

#include "core/Hello.h"

int main(int argc, char* argv[]) {
  QGuiApplication app(argc, argv);

  Hello hello;
  std::cout << hello.greet() << "\n";

  QQmlApplicationEngine engine;
  engine.load(QUrl(QStringLiteral("qrc:/Main.qml")));

  if (engine.rootObjects().isEmpty()) {
    return -1;
  }

  return app.exec();
}
