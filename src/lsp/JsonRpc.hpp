#pragma once

#include <QJsonObject>
#include <QJsonArray>

namespace JsonRpc {

inline QJsonObject request(const QString& method, const QJsonObject& params, int& requestId) {
  QJsonObject msg;
  msg["jsonrpc"] = "2.0";
  msg["id"] = requestId++;
  msg["method"] = method;
  if (!params.isEmpty()) msg["params"] = params;
  return msg;
}

inline QJsonObject notification(const QString& method, const QJsonObject& params = {}) {
  QJsonObject msg;
  msg["jsonrpc"] = "2.0";
  msg["method"] = method;
  if (!params.isEmpty()) msg["params"] = params;
  return msg;
}

inline QJsonObject textDocument(const QString& uri) {
  return QJsonObject{{"uri", uri}};
}

inline QJsonObject textDocumentItem(const QString& uri, const QString& languageId, const QString& text) {
  return QJsonObject{
    {"uri", uri},
    {"languageId", languageId},
    {"version", 1},
    {"text", text}
  };
}

inline QJsonObject versionedTextDocument(const QString& uri, int version) {
  return QJsonObject{
    {"uri", uri},
    {"version", version}
  };
}

inline QJsonObject fullContentChange(const QString& text) {
  return QJsonObject{{"text", text}};
}

inline QJsonObject semanticTokenCapabilities() {
  QJsonObject semanticTokens;
  semanticTokens["dynamicRegistration"] = false;
  semanticTokens["tokenTypes"] = QJsonArray({"type", "function", "variable", "parameter", "class", "enum", "interface", "struct", "typeParameter"});
  semanticTokens["tokenModifiers"] = QJsonArray({"declaration", "definition", "readonly", "static"});
  semanticTokens["formats"] = QJsonArray({"relative"});
  return semanticTokens;
}

}
