#ifndef CMD_H
#define CMD_H

#include "message.hpp"
#include <utility>

struct Cmd {
  enum class CmdType { None, Quit, SendMessage };

  CmdType type = CmdType::None;
  Msg msg;

  static Cmd NoneCmd() { return Cmd{CmdType::None, Msg::MsgType::None}; }

  static Cmd Quit() { return Cmd{CmdType::Quit, Msg::MsgType::None}; }

  static Cmd Send(Msg m) { return Cmd{CmdType::SendMessage, std::move(m)}; }
};

#endif // CMD_H