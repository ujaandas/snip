#pragma once

#include <QJsonArray>
#include <QList>

struct SemanticToken {
  int line;
  int character;
  int length;
  int type;
  int modifiers;
};

class SemanticTokens {
 public:
  static QList<SemanticToken> parse(const QJsonArray& data);
};
