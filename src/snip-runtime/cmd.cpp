#include "cmd.hpp"
#include "file/file.hpp"
#include "msg.hpp"
#include <cerrno>
#include <chrono>
#include <climits>
#include <cstring>
#include <exception>
#include <sys/ioctl.h>
#include <thread>

namespace snip::runtime {

Cmd Noop() {
  return []() -> std::optional<Msg> { return std::nullopt; };
}

std::vector<Cmd> Batch(std::vector<Cmd> cmds) { return cmds; }

Cmd Sequence(std::vector<Cmd> cmds) {
  return [cmds = std::move(cmds)]() mutable -> std::optional<Msg> {
    std::optional<Msg> lastMsg;
    for (auto &cmd : cmds) {
      if (!cmd) {
        continue;
      }
      if (auto maybeMsg = cmd()) {
        lastMsg = std::move(*maybeMsg);
      }
    }
    return lastMsg;
  };
}

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

Cmd Quit() {
  return []() -> std::optional<Msg> { return QuitMsg{}; };
}

Cmd ReadWindowSize(int fd) {
  return [fd]() -> std::optional<Msg> {
    struct winsize w;
    if (ioctl(fd, TIOCGWINSZ, &w) == -1) {
      return IOErrorMsg{"ioctl", "window-size", std::strerror(errno)};
    }
    return WindowSizeMsg{w.ws_col, w.ws_row};
  };
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