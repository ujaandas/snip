#ifndef MESSAGE_H
#define MESSAGE_H

struct Msg {
  enum class MsgType { Quit, Keypress, Increment };

  MsgType type;
  char key;
  int inc = 1;

  static Msg Quit() { return Msg{MsgType::Quit}; }
  static Msg Keypress(char c) { return Msg{MsgType::Keypress, .key = c}; }
  static Msg Increment(int i) { return Msg{MsgType::Increment, .inc = i}; }
};

#endif // MESSAGE_H
