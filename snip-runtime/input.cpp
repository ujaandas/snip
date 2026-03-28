#include "input.hpp"

#include <array>
#include <cctype>
#include <unistd.h>

namespace snip::runtime::input {
namespace {

std::optional<KeyPressMsg> parseEscape(const std::string& raw) {
  if (raw == "\x1b") {
    return KeyPressMsg{.code = KeyCode::Escape, .raw = raw};
  }

  if (raw == "\x1b[A") {
    return KeyPressMsg{.code = KeyCode::Up, .raw = raw};
  }
  if (raw == "\x1b[B") {
    return KeyPressMsg{.code = KeyCode::Down, .raw = raw};
  }
  if (raw == "\x1b[C") {
    return KeyPressMsg{.code = KeyCode::Right, .raw = raw};
  }
  if (raw == "\x1b[D") {
    return KeyPressMsg{.code = KeyCode::Left, .raw = raw};
  }
  if (raw == "\x1b[H") {
    return KeyPressMsg{.code = KeyCode::Home, .raw = raw};
  }
  if (raw == "\x1b[F") {
    return KeyPressMsg{.code = KeyCode::End, .raw = raw};
  }
  if (raw == "\x1b[5~") {
    return KeyPressMsg{.code = KeyCode::PageUp, .raw = raw};
  }
  if (raw == "\x1b[6~") {
    return KeyPressMsg{.code = KeyCode::PageDown, .raw = raw};
  }
  if (raw == "\x1b[3~") {
    return KeyPressMsg{.code = KeyCode::DeleteKey, .raw = raw};
  }
  if (raw == "\x1b[2~") {
    return KeyPressMsg{.code = KeyCode::Insert, .raw = raw};
  }

  if (raw.size() == 2 && raw[0] == 0x1b) {
    return KeyPressMsg{
        .code = KeyCode::Rune,
        .rune = raw[1],
        .alt = true,
        .raw = raw,
    };
  }

  return KeyPressMsg{.code = KeyCode::Unknown, .raw = raw};
}

} // namespace

std::optional<KeyPressMsg> parseKeySequence(const std::string& raw) {
  if (raw.empty()) {
    return std::nullopt;
  }

  const unsigned char c = raw[0];

  if (c == 0x1b) {
    return parseEscape(raw);
  }

  if (c == '\n' || c == '\r') {
    return KeyPressMsg{.code = KeyCode::Enter, .raw = raw};
  }

  if (c == '\t') {
    return KeyPressMsg{.code = KeyCode::Tab, .raw = raw};
  }

  if (c == 0x7f) {
    return KeyPressMsg{.code = KeyCode::Backspace, .raw = raw};
  }

  if (std::isprint(c) != 0) {
    return KeyPressMsg{.code = KeyCode::Rune, .rune = static_cast<char>(c), .raw = raw};
  }

  if (c < 0x20) {
    return KeyPressMsg{
        .code = KeyCode::Rune,
        .rune = static_cast<char>('a' + (c - 1)),
        .ctrl = true,
        .raw = raw,
    };
  }

  return KeyPressMsg{.code = KeyCode::Unknown, .raw = raw};
}

std::optional<KeyPressMsg> readKeyPress(int fd) {
  char ch = '\0';
  const ssize_t firstRead = ::read(fd, &ch, 1);
  if (firstRead <= 0) {
    return std::nullopt;
  }

  std::string raw;
  raw.push_back(ch);

  if (ch == 0x1b) {
    std::array<char, 8> tail{};
    const ssize_t n = ::read(fd, tail.data(), tail.size());
    if (n > 0) {
      raw.append(tail.data(), n);
    }
  }

  return parseKeySequence(raw);
}

} // namespace snip::runtime::input
