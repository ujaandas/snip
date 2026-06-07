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
#include "Theme.hpp"

TreeSitter::TreeSitter(QObject *parent) : QObject(parent) {}

void TreeSitter::setTheme(Theme *theme) {
  theme_ = theme;
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
  if (!theme_) return;

  // Core captures - Keywords
  captureColors_["keyword"] = theme_->hlKeyword;
  captureColors_["keyword.function"] = theme_->hlKeyword;
  captureColors_["keyword.operator"] = theme_->hlOperator;
  captureColors_["keyword.return"] = theme_->hlKeyword;
  captureColors_["keyword.import"] = theme_->hlKeyword;
  captureColors_["keyword.export"] = theme_->hlKeyword;
  captureColors_["keyword.conditional"] = theme_->hlKeyword;
  captureColors_["keyword.repeat"] = theme_->hlKeyword;
  captureColors_["keyword.exception"] = theme_->hlKeyword;
  captureColors_["keyword.storage"] = theme_->hlKeyword;
  captureColors_["keyword.directive"] = theme_->hlKeyword;
  captureColors_["keyword.debug"] = theme_->hlKeyword;
  captureColors_["conditional"] = theme_->hlKeyword;
  captureColors_["repeat"] = theme_->hlKeyword;
  captureColors_["include"] = theme_->hlKeyword;
  captureColors_["exception"] = theme_->hlKeyword;
  captureColors_["storageclass"] = theme_->hlKeyword;
  captureColors_["label"] = theme_->hlKeyword;

  // Types
  captureColors_["type"] = theme_->hlType;
  captureColors_["type.builtin"] = theme_->hlType;
  captureColors_["type.definition"] = theme_->hlType;
  captureColors_["type.qualifier"] = theme_->hlKeyword;
  captureColors_["type.parameter"] = theme_->hlType;
  captureColors_["namespace"] = theme_->hlType;
  captureColors_["class"] = theme_->hlType;
  captureColors_["struct"] = theme_->hlType;
  captureColors_["enum"] = theme_->hlType;
  captureColors_["union"] = theme_->hlType;
  captureColors_["interface"] = theme_->hlType;
  captureColors_["typedef"] = theme_->hlType;

  // Functions
  captureColors_["function"] = theme_->hlFunction;
  captureColors_["function.builtin"] = theme_->hlFunction;
  captureColors_["function.call"] = theme_->hlFunction;
  captureColors_["function.macro"] = theme_->hlFunction;
  captureColors_["function.method"] = theme_->hlFunction;
  captureColors_["function.method.call"] = theme_->hlFunction;
  captureColors_["function.definition"] = theme_->hlFunction;
  captureColors_["function.special"] = theme_->hlFunction;
  captureColors_["macro"] = theme_->hlFunction;
  captureColors_["method"] = theme_->hlFunction;
  captureColors_["method.call"] = theme_->hlFunction;
  captureColors_["constructor"] = theme_->hlFunction;
  captureColors_["destructor"] = theme_->hlFunction;

  // Variables
  captureColors_["variable"] = theme_->hlVariable;
  captureColors_["variable.builtin"] = theme_->hlVariable;
  captureColors_["variable.parameter"] = theme_->hlParameter;
  captureColors_["variable.member"] = theme_->hlVariable;
  captureColors_["parameter"] = theme_->hlParameter;
  captureColors_["field"] = theme_->hlVariable;
  captureColors_["property"] = theme_->hlVariable;
  captureColors_["member"] = theme_->hlVariable;
  captureColors_["constant"] = theme_->hlVariable;
  captureColors_["constant.builtin"] = theme_->hlKeyword;
  captureColors_["constant.macro"] = theme_->hlVariable;
  captureColors_["self"] = theme_->hlKeyword;
  captureColors_["this"] = theme_->hlKeyword;

  // Literals
  captureColors_["number"] = theme_->hlNumber;
  captureColors_["number.float"] = theme_->hlNumber;
  captureColors_["float"] = theme_->hlNumber;
  captureColors_["string"] = theme_->hlString;
  captureColors_["string.documentation"] = theme_->hlComment;
  captureColors_["string.regex"] = theme_->hlString;
  captureColors_["string.escape"] = theme_->hlKeyword;
  captureColors_["string.special"] = theme_->hlString;
  captureColors_["string.special.symbol"] = theme_->hlString;
  captureColors_["string.special.url"] = theme_->hlString;
  captureColors_["character"] = theme_->hlString;
  captureColors_["character.special"] = theme_->hlKeyword;
  captureColors_["boolean"] = theme_->hlKeyword;
  captureColors_["null"] = theme_->hlKeyword;
  captureColors_["undefined"] = theme_->hlKeyword;
  captureColors_["literal"] = theme_->hlString;

  // Comments
  captureColors_["comment"] = theme_->hlComment;
  captureColors_["comment.documentation"] = theme_->hlComment;
  captureColors_["comment.error"] = theme_->hlError;
  captureColors_["comment.warning"] = theme_->hlWarning;
  captureColors_["comment.todo"] = theme_->hlTodo;
  captureColors_["comment.note"] = theme_->hlInfo;

  // Operators & Punctuation
  captureColors_["operator"] = theme_->hlOperator;
  captureColors_["punctuation"] = theme_->hlPunctuation;
  captureColors_["punctuation.delimiter"] = theme_->hlPunctuation;
  captureColors_["punctuation.bracket"] = theme_->hlPunctuation;
  captureColors_["punctuation.special"] = theme_->hlPunctuation;
  captureColors_["punctuation.definition.string"] = theme_->hlString;
  captureColors_["punctuation.definition.comment"] = theme_->hlComment;

  // Tags (for HTML/XML/markup)
  captureColors_["tag"] = theme_->hlTag;
  captureColors_["tag.builtin"] = theme_->hlTag;
  captureColors_["tag.delimiter"] = theme_->hlPunctuation;
  captureColors_["attribute"] = theme_->hlAttribute;
  captureColors_["attribute.builtin"] = theme_->hlAttribute;

  // Preprocessor/Directives
  captureColors_["preproc"] = theme_->hlKeyword;
  captureColors_["define"] = theme_->hlKeyword;
  captureColors_["preprocessor"] = theme_->hlKeyword;
  captureColors_["directive"] = theme_->hlKeyword;

  // Special
  captureColors_["module"] = theme_->hlModule;
  captureColors_["import"] = theme_->hlModule;
  captureColors_["export"] = theme_->hlModule;
  captureColors_["escape"] = theme_->hlKeyword;
  captureColors_["format"] = theme_->hlKeyword;
  captureColors_["repeat.intro"] = theme_->hlKeyword;

  // Diff
  captureColors_["diff.plus"] = theme_->hlDiffAdd;
  captureColors_["diff.minus"] = theme_->hlDiffDelete;
  captureColors_["diff.delta"] = theme_->hlDiffChange;

  // Conceal/Noise
  captureColors_["conceal"] = theme_->textMuted;
  captureColors_["noise"] = theme_->textMuted;
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

    // get color for capture (fallback to text color if not in theme)
    QColor color = captureColors_.value(captureName, theme_ ? theme_->textPrimary : QColor("#cdd6f4"));

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
