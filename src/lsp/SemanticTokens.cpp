#include "SemanticTokens.hpp"

#include "Log.hpp"

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
