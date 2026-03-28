#include "cmd.hpp"
#include "file/file.hpp"
#include <climits>
#include <exception>

namespace snip::runtime {

Cmd Quit() {
  return []() -> std::optional<Msg> { return QuitMsg{}; };
}

Cmd ReadFile(std::string path) {
  return [path]() -> std::optional<Msg> {
    try {
      std::vector<std::string> fileData = File::readRange(path, 0, INT_MAX);
      return FileLoadedMsg{path, std::move(fileData)};

    } catch (const std::exception &e) {
      return IOErrorMsg{"read", path, e.what()};
    }
  };
}

Cmd WriteFile(std::string path, const std::vector<std::string> &buffer) {
  return [path, buffer]() -> std::optional<Msg> {
    try {
      const std::size_t bytes = File::writeAll(path, buffer);
      return FileSavedMsg{path, bytes, buffer.size()};
    } catch (const std::exception &e) {
      return IOErrorMsg{"write", path, e.what()};
    }
  };
}

} // namespace snip::runtime