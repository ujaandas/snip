#include "Buffer.h"

#include <QtCore/qtypes.h>

Buffer::Buffer(std::string text) : text_(QString::fromStdString(text)) {}

QStringView Buffer::text() const { return text_; }

qsizetype Buffer::scrollOffset() const { return scrollOffset_; }

void Buffer::setScrollOffset(qsizetype offset) { scrollOffset_ = offset; }
