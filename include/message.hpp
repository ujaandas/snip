#ifndef MESSAGE_H
#define MESSAGE_H

struct MsgPayload {
  char key;
  int inc = 1;
};

struct Msg {
  enum class MsgType { None, Quit, Keypress, Increment };

  MsgType type = MsgType::None;
  MsgPayload payload = {};
};

#endif // MESSAGE_H
