#include "cmd.hpp"
#include "program.hpp"

Cmd Quit(Program &p) {
  return [&p]() -> std::optional<Msg> {
    p.requestQuit();
    return std::nullopt;
  };
}

Cmd Send(Msg m) {
  return [m]() -> std::optional<Msg> { return m; };
}

Cmd OpenFile(std::string path) {
  return [path]() -> std::optional<Msg> { return FilepathMsg{path}; };
}
