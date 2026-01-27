/*
A "message" is simply a block of information. That's it. It is data.
*/

#ifndef MESSAGE_H
#define MESSAGE_H

#include <string>
#include <variant>

struct QuitMsg {};

struct IncrementMsg {
  int i;
};

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

using Msg = std::variant<QuitMsg, IncrementMsg, KeypressMsg,
                         WindowDimensionsMsg, FilepathMsg>;

#endif // MESSAGE_H
