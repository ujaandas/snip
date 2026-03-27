#include "cmd.hpp"
#include "file/file.hpp"
#include "msg.hpp"
#include <climits>
#include <chrono>
#include <exception>
#include <thread>

namespace snip {

Cmd Emit(Msg msg) {
  return [msg = std::move(msg)]() mutable -> std::optional<Msg> {
    return std::move(msg);
  };
}

Cmd DelayMs(int milliseconds, Msg msg) {
  return [milliseconds, msg = std::move(msg)]() mutable -> std::optional<Msg> {
    std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
    return std::move(msg);
  };
}

Cmd Tick(std::string id, std::uint64_t seq, int milliseconds) {
  return [id = std::move(id), seq, milliseconds]() -> std::optional<Msg> {
    std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
    return TickMsg{id, seq};
  };
}

Cmd Quit() { return []() -> std::optional<Msg> { return QuitMsg{}; }; }

Cmd ReadFile(std::string path) {
  // Return the lambda that the thread pool will execute
  return [path]() -> std::optional<Msg> {
    try {
      // This happens on a background thread.
      std::vector<std::string> fileData = File::readRange(path, 0, INT_MAX);

      // Wrap the data in our custom message and send it back to the UI.
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

} // namespace snip