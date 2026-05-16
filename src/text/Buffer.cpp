#include "Buffer.h"

Buffer::Buffer(QString text) : text_(text) {}

QStringView Buffer::text() const { return text_; }

qsizetype Buffer::scrollOffset() const { return scrollOffset_; }

void Buffer::setScrollOffset(qsizetype offset) { scrollOffset_ = offset; }
