#pragma once

#include <cstddef>
#include <string>
#include <vector>

namespace snip::editor {

class Piece {
private:
  const std::string* buffer;
  std::size_t offset;
  std::size_t length;

  // Only PieceTable can create pieces
  friend class PieceTable;

  Piece(const std::string* buffer, std::size_t offset, std::size_t length)
      : buffer(buffer), offset(offset), length(length) {}

  inline Piece left(std::size_t len) const {
    return Piece{buffer, offset, len};
  }

  inline Piece right(std::size_t split) const {
    return Piece{buffer, offset + split, length - split};
  }
};

class PieceTable {
private:
  const std::string original;
  std::string add;
  std::vector<Piece> pieces;

public:
  explicit PieceTable(std::string initial);
  void insert(std::size_t pos, char c);
  std::string toString() const;
};
} // namespace snip::editor