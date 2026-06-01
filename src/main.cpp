#include <QtQml/qqml.h>

#include <QtCore/QUrl>
#include <QtGui/QGuiApplication>
#include <QtQml/QQmlApplicationEngine>

#include "EditorView.hpp"

int main(int argc, char* argv[]) {
  QGuiApplication app(argc, argv);

  qmlRegisterType<EditorView>("MyEditor", 1, 0, "EditorView");

  QQmlApplicationEngine engine;
  engine.load(QUrl(QStringLiteral("qrc:/Main.qml")));

  if (engine.rootObjects().isEmpty()) {
    return -1;
  }

  return app.exec();
}
