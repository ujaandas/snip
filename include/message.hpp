/*
A "message" is simply a block of information. That's it.
Messages are used in the update loop, and can also be contained within commands.
Message use and structure should emulate commands.
*/

#ifndef MESSAGE_H
#define MESSAGE_H

#include <string>
struct Msg {
  // These should all be nouns
  enum class MsgType { Quit, Keypress, Integer, Text };

  // TODO: Look into using std::variant so we can define a separate struct for
  // each one and not share data like this
  MsgType type;
  char key;
  int i = 1;
  std::string text;

  // Static helper functions to build relevant MsgTypes quicker
  static Msg Keypress(char c) { return Msg{MsgType::Keypress, c, 0}; }
  static Msg Write(std::string text) {
    return Msg{MsgType::Text, '\0', 0, text};
  }
  static Msg Increment(int i) { return Msg{MsgType::Integer, '\0', i}; }
  static Msg Quit() { return Msg{MsgType::Quit, 0, 1}; }
};

#endif // MESSAGE_H
