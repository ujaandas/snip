#pragma once

#include <QJsonArray>
#include <QList>

#include "Theme.hpp"

struct SemanticToken {
  int line;
  int character;
  int length;
  int type;
  int modifiers;
};

// LSP Semantic Token Types (order matches LSP spec)
// See: https://microsoft.github.io/language-server-protocol/specifications/lsp/3.17/specification/#textDocument_semanticTokens
enum class TokenType {
  Namespace = 0,      // namespace, module, package
  Type = 1,           // class, enum, interface, struct, type parameter
  Class = 2,          // class
  Enum = 3,           // enum
  Interface = 4,      // interface
  Struct = 5,         // struct
  TypeParameter = 6,  // type parameter / generic parameter
  Parameter = 7,      // function/method parameter
  Variable = 8,       // variable
  Property = 9,       // property / field
  EnumMember = 10,    // enum member
  Event = 11,         // event
  Function = 12,      // function
  Method = 13,        // method
  Macro = 14,         // macro
  Keyword = 15,       // keyword
  Modifier = 16,      // modifier (static, async, etc)
  Comment = 17,       // comment
  String = 18,        // string literal
  Number = 19,        // number literal
  Regexp = 20,        // regular expression
  Operator = 21,      // operator
  Unknown = 255
};

class SemanticTokens {
 public:
  static QList<SemanticToken> parse(const QJsonArray& data);
  static QString typeToString(int type);
  static QColor typeToColor(int type);

  static void setTheme(Theme *theme);

 private:
  static Theme *theme_;
};
