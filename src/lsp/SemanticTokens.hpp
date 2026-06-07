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

// LSP token types (order matters)
enum class TokenType {
  Type = 0,
  Function = 1,
  Variable = 2,
  Parameter = 3,
  Class = 4,
  Enum = 5,
  Interface = 6,
  Struct = 7,
  TypeParameter = 8,
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
