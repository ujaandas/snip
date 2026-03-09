#ifndef MESSAGE_H
#define MESSAGE_H

#include <string>
#include <variant>

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

/*
A "message" is simply a block of information. That's it. It is data.
*/
using Msg = std::variant<KeypressMsg, WindowDimensionsMsg, FilepathMsg>;

#endif // MESSAGE_H
