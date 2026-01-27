#include "message.hpp"
#include <string>

/*
A "command" is not technically a command - it is not runnable.
In insteads represents a "deferred action" that the runtime should act upon.
We use this to defer operations from our update loop to keep it pure, which
makes testing a lot easier.
Any operation the app wants to perform in the update loop, ie; any "reactive"
operation, should be defined here with a Command.
Simply put, commands are "things that the program does".
*/

#ifndef CMD_H
#define CMD_H

// A deferred action from the core event handling loop.
struct QuitCmd {};
struct NoneCmd {};
struct SendMessageCmd {
  Msg msg;
};
struct OpenFileCmd {
  std::string path;
};
using Cmd = std::variant<QuitCmd, NoneCmd, SendMessageCmd, OpenFileCmd>;

#endif // CMD_H