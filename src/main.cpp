#include <QApplication>
#include <QLabel>
#include <QVBoxLayout>
#include <QWidget>
#include <iostream>

#include "core/Hello.h"

int main(int argc, char* argv[]) {
  QApplication app(argc, argv);

  Hello hello;
  std::cout << hello.greet() << "\n";

  QWidget window;
  window.setWindowTitle("Hello World");
  window.resize(300, 100);

  QVBoxLayout* layout = new QVBoxLayout(&window);
  QLabel* label = new QLabel("Hello World");
  label->setAlignment(Qt::AlignCenter);
  layout->addWidget(label);

  window.show();
  return app.exec();
}
