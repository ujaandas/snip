#include "TreeSitter.hpp"

#include <QFile>
#include <QTextStream>
#include <QDir>
#include <dlfcn.h>

// C API subset we need
extern "C" {
typedef struct TSLanguage TSLanguage;
typedef struct TSParser TSParser;
typedef struct TSQuery TSQuery;
typedef struct TSQueryCursor TSQueryCursor;
typedef struct TSTree TSTree;
typedef struct TSNode TSNode;

TSParser* ts_parser_new(void);
void ts_parser_delete(TSParser* parser);
bool ts_parser_set_language(TSParser* parser, const TSLanguage* language);
TSTree* ts_parser_parse_string(TSParser* parser, const TSTree* old_tree, const char* string, uint32_t len);
void ts_tree_delete(TSTree* tree);
TSNode ts_tree_root_node(const TSTree* tree);

TSQuery* ts_query_new(const TSLanguage* language, const char* source, uint32_t source_len, uint32_t* error_offset, int32_t* error_type);
void ts_query_delete(TSQuery* query);
uint32_t ts_query_capture_count(const TSQuery* query);
const char* ts_query_capture_name_for_id(const TSQuery* query, uint32_t id, uint32_t* length);

TSQueryCursor* ts_query_cursor_new(void);
void ts_query_cursor_delete(TSQueryCursor* cursor);
void ts_query_cursor_exec(TSQueryCursor* cursor, const TSQuery* query, TSNode node);
bool ts_query_cursor_next_capture(TSQueryCursor* cursor, uint32_t* capture_index, TSNode* capture_node);
uint32_t ts_node_start_byte(TSNode node);
uint32_t ts_node_end_byte(TSNode node);
}

TreeSitter::TreeSitter(QObject* parent) : QObject(parent) {
  setupCaptureColors();
}

TreeSitter::~TreeSitter() {
  if (highlightsQuery_) ts_query_delete(highlightsQuery_);
  if (parser_) ts_parser_delete(parser_);
  if (grammarHandle_) dlclose(grammarHandle_);
}

void TreeSitter::setupCaptureColors() {
  // VS Code dark theme inspired colors
  captureColors_["keyword"] = QColor("#C586C0");      // purple (if, for, return)
  captureColors_["operator"] = QColor("#D4D4D4");      // white (+, -, =)
  captureColors_["string"] = QColor("#CE9178");      // orange-ish ("hello")
  captureColors_["comment"] = QColor("#6A9955");     // green (// comment)
  captureColors_["number"] = QColor("#B5CEA8");      // light green (42)
  captureColors_["function"] = QColor("#DCDCAA");      // yellow (myFunc)
  captureColors_["type"] = QColor("#4EC9B0");          // teal (MyClass)
  captureColors_["variable"] = QColor("#9CDCFE");     // light blue (myVar)
  captureColors_["punctuation"] = QColor("#D4D4D4");  // white ({, }, (, ))
}

QString TreeSitter::readQueryFile(const QString& queryDir, const QString& filename) {
  QFile file(QDir(queryDir).filePath(filename));
  if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) return QString();
  QTextStream in(&file);
  return in.readAll();
}

bool TreeSitter::loadLanguage(const QString& parserSoPath, const QString& queryDir) {
  // dlopen the grammar .so
  grammarHandle_ = dlopen(parserSoPath.toUtf8().constData(), RTLD_LAZY);
  if (!grammarHandle_) {
    return false;
  }

  // get the language function (name pattern: tree_sitter_<lang>)
  QString langName = QFileInfo(parserSoPath).baseName().remove("libtree-sitter-");
  QString symbolName = "tree_sitter_" + langName;

  auto* langFunc = reinterpret_cast<const TSLanguage* (*)()>(dlsym(grammarHandle_, symbolName.toUtf8().constData()));
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
    int32_t error_type = 0;
    highlightsQuery_ = ts_query_new(
      language_,
      highlightsScm.toUtf8().constData(),
      highlightsScm.length(),
      &error_offset,
      &error_type
    );
  }

  return true;
}

void TreeSitter::highlight(QTextDocument* doc) {
  if (!parser_ || !highlightsQuery_ || !doc) return;

  // TODO: Implement highlighting in next chunk
  // 1. get document text as UTF-8
  // 2. parse with tree-sitter
  // 3. execute highlights query
  // 4. apply colors to QTextDocument
}
