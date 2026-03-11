#include "cmd.hpp"
#include "file/file.hpp"
#include "msg.hpp"
#include <climits>
#include <exception>

namespace snip {

Cmd OpenFile(std::string path) {
  // Return the lambda that the thread pool will execute
  return [path]() -> std::optional<Msg> {
    try {
      // This happens on a background thread!
      File f(path);
      std::vector<std::string> fileData = f.readRange(0, INT_MAX);

      //  Wrap the data in our custom message and send it back to the UI
      return FileLoadedMsg{path, std::move(fileData)};

    } catch (const std::exception &e) {
      // If the file doesn't exist or we lack permissions
      return ErrorMsg{e.what()};
    }
  };
}

} // namespace snip