#pragma once

#include "msg.hpp"
#include <optional>
#include <string>

namespace snip::runtime::input {

// Parse a raw terminal key sequence into a richer key message
std::optional<KeyPressMsg> parseKeySequence(const std::string &raw);

// Read and parse one key sequence from a non-blocking fd
std::optional<KeyPressMsg> readKeyPress(int fd);

} // namespace snip::runtime::input
