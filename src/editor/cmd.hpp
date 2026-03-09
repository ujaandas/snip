#pragma once

#include "../framework/cmd.hpp"
#include <string>

namespace snip {

// Command factory declarations
Cmd OpenFile(std::string path);
Cmd SaveFile(std::string path, const std::vector<std::string> &buffer);

} // namespace snip