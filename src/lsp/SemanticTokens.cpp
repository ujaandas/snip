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
  if (!theme_) {
    // Fallback to Catppuccin Mocha defaults if no theme set
    switch (type) {
      case 0: return QColor("#b4befe");   // namespace - Lavender
      case 1: return QColor("#f9e2af");   // type - Yellow
      case 2: return QColor("#f9e2af");   // class - Yellow
      case 3: return QColor("#f9e2af");   // enum - Yellow
      case 4: return QColor("#f9e2af");   // interface - Yellow
      case 5: return QColor("#f9e2af");   // struct - Yellow
      case 6: return QColor("#f9e2af");   // typeParameter - Yellow
      case 7: return QColor("#f2cdcd");   // parameter - Flamingo
      case 8: return QColor("#f2cdcd");   // variable - Flamingo
      case 9: return QColor("#f2cdcd");   // property - Flamingo
      case 10: return QColor("#f9e2af");  // enumMember - Yellow
      case 11: return QColor("#fab387");  // event - Peach
      case 12: return QColor("#89dceb");  // function - Sky
      case 13: return QColor("#89dceb");  // method - Sky
      case 14: return QColor("#cba6f7");  // macro - Mauve
      case 15: return QColor("#cba6f7");  // keyword - Mauve
      case 16: return QColor("#cba6f7");  // modifier - Mauve
      case 17: return QColor("#7f849c");  // comment - Overlay1
      case 18: return QColor("#a6e3a1");  // string - Green
      case 19: return QColor("#fab387");  // number - Peach
      case 20: return QColor("#f5c2e7");  // regexp - Pink
      case 21: return QColor("#cdd6f4");  // operator - Text
      default: return QColor("#cdd6f4");  // unknown - Text
    }
  }

  switch (type) {
    case 0: return theme_->hlModule;       // namespace
    case 1: return theme_->hlType;           // type
    case 2: return theme_->hlType;           // class
    case 3: return theme_->hlType;           // enum
    case 4: return theme_->hlType;           // interface
    case 5: return theme_->hlType;           // struct
    case 6: return theme_->hlType;           // typeParameter
    case 7: return theme_->hlParameter;      // parameter
    case 8: return theme_->hlVariable;       // variable
    case 9: return theme_->hlVariable;       // property
    case 10: return theme_->hlType;          // enumMember
    case 11: return theme_->hlNumber;        // event (peach)
    case 12: return theme_->hlFunction;       // function
    case 13: return theme_->hlFunction;       // method
    case 14: return theme_->hlFunction;       // macro
    case 15: return theme_->hlKeyword;       // keyword
    case 16: return theme_->hlKeyword;       // modifier
    case 17: return theme_->hlComment;      // comment
    case 18: return theme_->hlString;        // string
    case 19: return theme_->hlNumber;        // number
    case 20: return theme_->hlString;        // regexp
    case 21: return theme_->hlOperator;      // operator
    default: return theme_->textPrimary;     // unknown
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
