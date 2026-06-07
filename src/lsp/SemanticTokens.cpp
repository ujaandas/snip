#include "SemanticTokens.hpp"

#include <QColor>

#include "Log.hpp"

Theme *SemanticTokens::theme_ = nullptr;

void SemanticTokens::setTheme(Theme *theme) {
  theme_ = theme;
}

QString SemanticTokens::typeToString(int type) {
  switch (type) {
    case 0: return "type";
    case 1: return "function";
    case 2: return "variable";
    case 3: return "parameter";
    case 4: return "class";
    case 5: return "enum";
    case 6: return "interface";
    case 7: return "struct";
    case 8: return "typeParameter";
    default: return "unknown";
  }
}

QColor SemanticTokens::typeToColor(int type) {
  if (!theme_) {
    // Fallback to Catppuccin Mocha defaults if no theme set
    switch (type) {
      case 0: return QColor("#f9e2af");   // type - Yellow
      case 1: return QColor("#89dceb");   // function - Sky
      case 2: return QColor("#f2cdcd");   // variable - Flamingo
      case 3: return QColor("#f2cdcd");   // parameter - Flamingo
      case 4: return QColor("#f9e2af");   // class - Yellow
      case 5: return QColor("#f9e2af");   // enum - Yellow
      case 6: return QColor("#f9e2af");   // interface - Yellow
      case 7: return QColor("#f9e2af");   // struct - Yellow
      case 8: return QColor("#f9e2af");   // typeParameter - Yellow
      default: return QColor("#cdd6f4");  // unknown - Text
    }
  }

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
