/*
A "message" is simply a block of information. That's it. It is data.
*/
#include <string>
#include <variant>

#ifndef MESSAGE_H
#define MESSAGE_H

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
