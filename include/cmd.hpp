#ifndef CMD_H
#define CMD_H

#include "message.hpp"
#include <utility>

struct Cmd {
  enum class CmdType { Quit, SendMessage };

  CmdType type;
  Msg msg;

  static Cmd Quit() { return Cmd{CmdType::Quit}; }

  static Cmd Send(Msg m) { return Cmd{CmdType::SendMessage, std::move(m)}; }
};

#endif // CMD_H