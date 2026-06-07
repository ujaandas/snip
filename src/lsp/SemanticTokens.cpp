#include "SemanticTokens.hpp"

#include <QColor>

#include "Log.hpp"

Theme *SemanticTokens::theme_ = nullptr;

void SemanticTokens::setTheme(Theme *theme) {
  theme_ = theme;
}

QString SemanticTokens::typeToString(int type) {
  switch (type) {
    case 0: return "namespace";
    case 1: return "type";
    case 2: return "class";
    case 3: return "enum";
    case 4: return "interface";
    case 5: return "struct";
    case 6: return "typeParameter";
    case 7: return "parameter";
    case 8: return "variable";
    case 9: return "property";
    case 10: return "enumMember";
    case 11: return "event";
    case 12: return "function";
    case 13: return "method";
    case 14: return "macro";
    case 15: return "keyword";
    case 16: return "modifier";
    case 17: return "comment";
    case 18: return "string";
    case 19: return "number";
    case 20: return "regexp";
    case 21: return "operator";
    default: return "unknown";
  }
}

QColor SemanticTokens::typeToColor(int type) {
  switch (type) {
    case 0: return theme_->hlType;        // type
    case 1: return theme_->hlFunction;    // function
    case 2: return theme_->hlVariable;    // variable
    case 3: return theme_->hlParameter;   // parameter
    case 4: return theme_->hlType;        // class
    case 5: return theme_->hlType;        // enum
    case 6: return theme_->hlType;        // interface
    case 7: return theme_->hlType;        // struct
    case 8: return theme_->hlType;        // typeParameter
    default: return theme_->textPrimary;  // unknown
  }
}

QList<SemanticToken> SemanticTokens::parse(const QJsonArray& data) {
  QList<SemanticToken> tokens;
  const int count = data.size();
  if (count % 5 != 0) {
    Log::warning("Invalid semantic token data: size {} not divisible by 5", count);
    return tokens;
  }

  int currentLine = 0;
  int currentChar = 0;

  for (int i = 0; i < count; i += 5) {
    // line delta (relative to previous token)
    currentLine += data[i].toInt();
    // char delta (relative if same line, absolute if new line)
    int charDelta = data[i + 1].toInt();
    if (i > 0 && data[i].toInt() == 0) {
      currentChar += charDelta;
    } else {
      currentChar = charDelta;
    }

    tokens.append({
      currentLine,
      currentChar,
      data[i + 2].toInt(),   // length
      data[i + 3].toInt(),   // type
      data[i + 4].toInt()    // modifiers
    });
  }

  return tokens;
}
