/*
A "message" is simply a block of information. That's it.
Messages are used in the update loop, and can also be contained within commands.
Message use and structure should emulate commands.
*/

#ifndef MESSAGE_H
#define MESSAGE_H

struct Msg {
  enum class MsgType { Quit, Keypress, Increment };

  // TODO: Look into using std::variant so we can define a separate struct for
  // each one and not share data like this
  MsgType type;
  char key;
  int inc = 1;

  // Static helper functions to build relevant MsgTypes quicker
  static Msg Keypress(char c) { return Msg{MsgType::Keypress, c, 1}; }
  static Msg Increment(int i) { return Msg{MsgType::Increment, 0, i}; }
  static Msg Quit() { return Msg{MsgType::Quit, 0, 1}; }
};

#endif // MESSAGE_H
