#ifndef MESSAGE_H
#define MESSAGE_H

class Message {
public:
  virtual ~Message() = default;
};

struct KeypressMessage : Message {
  int key;
  KeypressMessage(int key) : key(key) {}
};

#endif // MESSAGE_H