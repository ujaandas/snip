#pragma once

#include <QString>
#include <QtDebug>
#include <format>

namespace std {

template <>
struct formatter<QString, char> : formatter<std::string_view, char> {
  auto format(const QString& value, format_context& ctx) const {
    auto utf8 = value.toStdString();
    return formatter<std::string_view, char>::format(utf8, ctx);
  }
};

}  // namespace std

class Log {
 public:
  template <typename... Args>
  static void info(std::format_string<Args...> fmt, Args&&... args) {
    auto msg = std::format(fmt, std::forward<Args>(args)...);
    qInfo() << QString::fromStdString(msg);
  }

  template <typename... Args>
  static void warning(std::format_string<Args...> fmt, Args&&... args) {
    auto msg = std::format(fmt, std::forward<Args>(args)...);
    qWarning() << QString::fromStdString(msg);
  }

  template <typename... Args>
  static void debug(std::format_string<Args...> fmt, Args&&... args) {
    auto msg = std::format(fmt, std::forward<Args>(args)...);
    qDebug() << QString::fromStdString(msg);
  }

  template <typename... Args>
  static void critical(std::format_string<Args...> fmt, Args&&... args) {
    auto msg = std::format(fmt, std::forward<Args>(args)...);
    qCritical() << QString::fromStdString(msg);
  }

  template <typename... Args>
  static void fatal(std::format_string<Args...> fmt, Args&&... args) {
    auto msg = std::format(fmt, std::forward<Args>(args)...);
    qFatal("%s", msg.c_str());
  }
};