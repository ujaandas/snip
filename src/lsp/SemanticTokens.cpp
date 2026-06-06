#include "SemanticTokens.hpp"

#include <QColor>

#include "Log.hpp"

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
  switch (type) {
    case 0: return QColor("#4EC9B0");   // type - teal
    case 1: return QColor("#DCDCAA");   // function - yellow
    case 2: return QColor("#9CDCFE");   // variable - light blue
    case 3: return QColor("#9CDCFE");   // parameter - light blue
    case 4: return QColor("#4EC9B0");   // class - teal
    case 5: return QColor("#4EC9B0");   // enum - teal
    case 6: return QColor("#4EC9B0");   // interface - teal
    case 7: return QColor("#4EC9B0");   // struct - teal
    case 8: return QColor("#4EC9B0");   // typeParameter - teal
    default: return QColor("#D4D4D4");  // unknown - default text
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
