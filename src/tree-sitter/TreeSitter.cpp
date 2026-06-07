#include "TreeSitter.hpp"

#include <QDir>
#include <QFile>
#include <QTextBlock>
#include <QTextCharFormat>
#include <QTextCursor>
#include <QTextDocument>
#include <QTextStream>
#include <dlfcn.h>
#include <tree_sitter/api.h>

#include "Log.hpp"

TreeSitter::TreeSitter(QObject *parent) : QObject(parent) {
  setupCaptureColors();
}

TreeSitter::~TreeSitter() {
  if (highlightsQuery_)
    ts_query_delete(highlightsQuery_);
  if (parser_)
    ts_parser_delete(parser_);
  if (grammarHandle_)
    dlclose(grammarHandle_);
}

void TreeSitter::setupCaptureColors() {
  // VS Code dark theme inspired colors
  captureColors_["keyword"] = QColor("#C586C0");     // purple (if, for, return)
  captureColors_["operator"] = QColor("#D4D4D4");    // white (+, -, =)
  captureColors_["string"] = QColor("#CE9178");      // orange-ish ("hello")
  captureColors_["comment"] = QColor("#6A9955");     // green (// comment)
  captureColors_["number"] = QColor("#B5CEA8");      // light green (42)
  captureColors_["function"] = QColor("#DCDCAA");    // yellow (myFunc)
  captureColors_["type"] = QColor("#4EC9B0");        // teal (MyClass)
  captureColors_["variable"] = QColor("#9CDCFE");    // light blue (myVar)
  captureColors_["punctuation"] = QColor("#D4D4D4"); // white ({, }, (, ))
}

QString TreeSitter::readQueryFile(const QString &queryDir,
                                  const QString &filename) {
  QFile file(QDir(queryDir).filePath(filename));
  if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    return QString();
  QTextStream in(&file);
  return in.readAll();
}

bool TreeSitter::loadLanguage(const QString &parserSoPath,
                              const QString &queryDir) {
  // dlopen the grammar .so
  grammarHandle_ = dlopen(parserSoPath.toUtf8().constData(), RTLD_LAZY);
  if (!grammarHandle_) {
    return false;
  }

  // get the language function (name pattern: tree_sitter_<lang>)
  QString langName =
      QFileInfo(parserSoPath).baseName().remove("libtree-sitter-");
  QString symbolName = "tree_sitter_" + langName;

  auto *langFunc = reinterpret_cast<const TSLanguage *(*)()>(
      dlsym(grammarHandle_, symbolName.toUtf8().constData()));
  if (!langFunc) {
    dlclose(grammarHandle_);
    grammarHandle_ = nullptr;
    return false;
  }

  language_ = langFunc();
  if (!language_) {
    dlclose(grammarHandle_);
    grammarHandle_ = nullptr;
    return false;
  }

  // create parser and set language
  parser_ = ts_parser_new();
  if (!ts_parser_set_language(parser_, language_)) {
    ts_parser_delete(parser_);
    parser_ = nullptr;
    dlclose(grammarHandle_);
    grammarHandle_ = nullptr;
    language_ = nullptr;
    return false;
  }

  // load highlights.scm query
  QString highlightsScm = readQueryFile(queryDir, "highlights.scm");
  if (highlightsScm.isEmpty()) {
    // try importing from another file
    highlightsScm = readQueryFile(queryDir, "../highlights.scm");
  }

  if (!highlightsScm.isEmpty()) {
    uint32_t error_offset = 0;
    TSQueryError error_type;

    highlightsQuery_ =
        ts_query_new(language_, highlightsScm.toUtf8().constData(),
                     highlightsScm.length(), &error_offset, &error_type);
  }

  return true;
}

void TreeSitter::highlight(QTextDocument *doc) {
  if (!parser_ || !highlightsQuery_ || !doc)
    return;

  // save modification state
  bool wasModified = doc->isModified();

  Log::info("tree-sitter: highlighting document with {} characters", doc->characterCount());

  // get document text as UTF-8
  QString text = doc->toPlainText();
  QByteArray utf8Text = text.toUtf8();

  // parse with tree-sitter
  TSTree *tree =
      ts_parser_parse_string(parser_,
                             nullptr, // no old tree for now (full re-parse)
                             utf8Text.constData(), utf8Text.size());
  if (!tree)
    return;

  // execute highlights query
  TSQueryCursor *cursor = ts_query_cursor_new();
  TSNode rootNode = ts_tree_root_node(tree);
  ts_query_cursor_exec(cursor, highlightsQuery_, rootNode);

  // apply colors to QTextDocument
  TSQueryMatch match;
  uint32_t captureIndex;
  int applied = 0;

  while (ts_query_cursor_next_capture(cursor, &match, &captureIndex)) {
    uint32_t nameLen = 0;
    const char *namePtr =
        ts_query_capture_name_for_id(highlightsQuery_, captureIndex, &nameLen);
    QString captureName = QString::fromUtf8(namePtr, nameLen);

    // get byte positions and convert to character positions
    TSNode captureNode = match.captures[captureIndex].node;

    uint32_t startByte = ts_node_start_byte(captureNode);
    uint32_t endByte = ts_node_end_byte(captureNode);

    // convert UTF-8 byte offset to QString character index
    int startPos = QString::fromUtf8(utf8Text.left(startByte)).length();
    int endPos = QString::fromUtf8(utf8Text.left(endByte)).length();

    // validate positions
    if (startPos < 0 || endPos > doc->characterCount())
      continue;

    // get color for capture
    QColor color = captureColors_.value(captureName, QColor("#D4D4D4"));

    // apply formatting
    QTextCursor textCursor(doc);
    textCursor.setPosition(startPos);
    textCursor.setPosition(endPos, QTextCursor::KeepAnchor);

    QTextCharFormat format;
    format.setForeground(color);
    textCursor.setCharFormat(format);
    applied++;
  }

  Log::info("tree-sitter: applied {} highlight captures", applied);

  // cleanup
  ts_query_cursor_delete(cursor);
  ts_tree_delete(tree);

  // restore modification state
  doc->setModified(wasModified);
}
