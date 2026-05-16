#pragma once

#include <QtCore/qobject.h>
#include <QtCore/qstringview.h>
#include <QtCore/qtypes.h>

class Buffer {
 private:
  QString text_;
  qsizetype scrollOffset_ = 0;

 public:
  explicit Buffer(std::string text);

  QStringView text() const;
  qsizetype scrollOffset() const;

  void setScrollOffset(qsizetype offset);
};