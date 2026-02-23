/*
A "message" is simply a block of information. That's it. It is data.
*/
#include <string>
#include <variant>

#ifndef MESSAGE_H
#define MESSAGE_H

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

using Msg = std::variant<KeypressMsg, WindowDimensionsMsg, FilepathMsg>;

#endif // MESSAGE_H
