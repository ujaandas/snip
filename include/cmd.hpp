#include "message.hpp"
#include <utility>

/*
A "command" is not technically a command - it is not runnable.
In insteads represents a "deferred action" that the runtime should act upon.
We use this to defer operations from our update loop to keep it pure, which
makes testing a lot easier.
Any operation the app wants to perform in the update loop, ie; any "reactive"
operation, should be defined here with a Command.
*/

#ifndef CMD_H
#define CMD_H

// A deferred action from the core event handling loop.
struct Cmd {
  enum class CmdType { Quit, SendMessage };

  CmdType type;
  Msg msg;

  // Static helper functions to build relevant CmdTypes quicker
  static Cmd Quit() { return Cmd{CmdType::Quit}; }
  static Cmd Send(Msg m) { return Cmd{CmdType::SendMessage, std::move(m)}; }
};

#endif // CMD_H