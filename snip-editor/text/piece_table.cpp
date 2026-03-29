#include "piece_table.hpp"

namespace snip::editor {

PieceTable::PieceTable(std::string initial) : original(std::move(initial)) {
  if (!original.empty()) {
    pieces.emplace_back(Piece(&original, 0, original.size()));
  }
}

void PieceTable::insert(std::size_t pos, char c) {
  add.push_back(c);

  Piece newPiece(&original, add.size() - 1, 1);

  if (pieces.empty()) {
    pieces.push_back(newPiece);
    return;
  }

  std::size_t cur = 0;
  auto it = pieces.begin();

  for (; it != pieces.end(); ++it) {
    if (cur + it->length >= pos) {
      break;
    }
    cur += it->length;
  }

  // End of piece table
  if (it == pieces.end()) {
    pieces.push_back(newPiece);
    return;
  }

  std::size_t offset = pos - cur;

  // Insert at beginning of piece
  if (offset == 0) {
    pieces.insert(it, newPiece);
  }
  // Insert at end of piece
  else if (offset == it->length) {
    pieces.insert(std::next(it), newPiece);
  }
  // Split piece
  else {
    Piece left = it->left(offset);
    Piece right = it->right(offset);

    it = pieces.erase(it);
    it = pieces.insert(it, left);
    it = pieces.insert(std::next(it), newPiece);
    pieces.insert(std::next(it), right);
  }
}

std::string PieceTable::toString() const {
  std::string result;
  for (const auto& p : pieces) {
    result.append(p.buffer->substr(p.offset, p.length));
  }
  return result;
}

} // namespace snip::editor
