#include "message.hpp"
#include <functional>

/*
A command represents a "deferred action" that the runtime should act upon.
We use this to defer operations from our update loop to keep it pure, which
makes testing a lot easier.
Essentially, it is a function that might _eventually_ produce a message.
*/

#ifndef CMD_H
#define CMD_H

class Program;

using Cmd = std::function<std::optional<Msg>()>;

Cmd Quit(Program &p);
Cmd Send(Msg m);
Cmd OpenFile(std::string path);

#endif // CMD_H