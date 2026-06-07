#pragma once

#include <QColor>
#include <QMap>
#include <QObject>
#include <tree_sitter/api.h>

#include "Theme.hpp"

class QTextDocument;

class TreeSitter : public QObject {
  Q_OBJECT;

public:
  explicit TreeSitter(QObject *parent = nullptr);
  ~TreeSitter();

  // load grammar from .so file and queries from directory
  bool loadLanguage(const QString &parserSoPath, const QString &queryDir);

  // highlight a document (full re-parse), TODO, incremental
  void highlight(QTextDocument *doc);

  // check if language is loaded
  bool isLoaded() const { return language_ != nullptr; }

  // set theme for syntax highlighting colors
  void setTheme(Theme *theme);

private:
  void *grammarHandle_ = nullptr;        // dlopen handle
  const TSLanguage *language_ = nullptr; // from dlsym
  TSParser *parser_ = nullptr;           // tree-sitter parser instance
  TSQuery *highlightsQuery_ = nullptr;   // compiled query (highlights + injections + tags)

  // map capture names to colors (e.g., "keyword" to yellow)
  QMap<QString, QColor> captureColors_;

  Theme *theme_ = nullptr;

  void setupCaptureColors();
  QString readQueryFile(const QString &queryDir, const QString &filename);
};