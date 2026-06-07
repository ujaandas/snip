#include <QtCore/qdir.h>
#include <QtQml/qqml.h>

#include <QCommandLineParser>
#include <QQuickStyle>
#include <QtCore/QUrl>
#include <QtGui/QFontDatabase>
#include <QtGui/QGuiApplication>
#include <QtQml/QQmlApplicationEngine>
#include <QtQml/QQmlContext>

#include "FileTree.hpp"
#include "Gutter.hpp"
#include "LspClient.hpp"
#include "SemanticTokens.hpp"
#include "StrictScroll.hpp"
#include "TabManager.hpp"
#include "Theme.hpp"
#include "TreeSitter.hpp"

int main(int argc, char* argv[]) {
  // Use a non-native Quick Controls style so QML control customization is
  // supported.
  QQuickStyle::setStyle("Basic");

  // Initialize app and engine
  QGuiApplication app(argc, argv);
  QCoreApplication::setApplicationName("parity");
  QQmlApplicationEngine engine;

  // Cmd line args
  QCommandLineParser parser;
  parser.setApplicationDescription(
      "Nix-backed IDE with hermetic developer environments. ");

  parser.addHelpOption();
  QCommandLineOption pathOption(QStringList() << "f" << "path",
                                "Open the specified path.", "pathname");
  parser.addOption(pathOption);

  parser.process(app);

  QString pathName = QDir::currentPath();
  const QStringList args = parser.positionalArguments();
  if (!args.isEmpty()) {
    pathName = args.first();
  }

  FileTree fileTree(pathName);
  TabManager tabs;

  qmlRegisterType<StrictScroll>("Snip.Editor", 1, 0, "StrictScroll");
  qmlRegisterType<Gutter>("Snip.Editor", 1, 0, "Gutter");

  Theme* theme = new Theme(&app);
  LspClient* lspClient = new LspClient(&app);
  tabs.setLspClient(lspClient);

  // load tree-sitter for C++
  TreeSitter* treeSitter = new TreeSitter(&app);

  // Set theme for syntax highlighting
  treeSitter->setTheme(theme);
  SemanticTokens::setTheme(theme);
  bool cppLoaded = treeSitter->loadLanguage(
    "/nix/store/sly5g0s5p9dnhlpnj7wpmh7mdqacjpfk-tree-sitter-cpp-0.23.4/parser",
    "/nix/store/sly5g0s5p9dnhlpnj7wpmh7mdqacjpfk-tree-sitter-cpp-0.23.4/queries"
  );
  if (cppLoaded) {
    tabs.setTreeSitter(treeSitter);
  }

  engine.rootContext()->setContextProperty("fileTree", &fileTree);
  engine.rootContext()->setContextProperty("tabManager", &tabs);
  engine.rootContext()->setContextProperty("theme", theme);
  engine.rootContext()->setContextProperty("lspClient", lspClient);
  engine.rootContext()->setContextProperty("treeSitter", treeSitter);

  // Load font
  QFontDatabase::addApplicationFont(":/assets/fonts/JetBrainsMono[wght].ttf");

  // Load main qml
  engine.load(QUrl(QStringLiteral("qrc:/Main.qml")));

  if (engine.rootObjects().isEmpty()) return -1;

  // Start LSP client
  lspClient->start(pathName);

  return app.exec();
}