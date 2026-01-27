/*
A "message" is simply a block of information. That's it.
Messages are used in the update loop, and can also be contained within commands.
Message use and structure should emulate commands.
Simply put, messages are "things that happen to the program".
*/

#ifndef MESSAGE_H
#define MESSAGE_H

#include <string>
#include <variant>

struct QuitMsg {};

struct KeypressMsg {
  char key;
};

struct WindowDimensionsMsg {
  int width;
  int height;
};

struct FilepathMsg {
  std::string path;
};

using Msg =
    std::variant<QuitMsg, KeypressMsg, WindowDimensionsMsg, FilepathMsg>;

#endif // MESSAGE_H
