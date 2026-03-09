#ifndef CMD_H
#define CMD_H

#include "msg.hpp"
#include <functional>

/*
A command represents a deferred, *external* action that _might_ produce a
message.
*/
using Cmd = std::function<std::optional<Msg>()>;

#endif // CMD_H