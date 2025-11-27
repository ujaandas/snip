#include <queue>
#include <memory>
#include "model.hpp"
#include "message.hpp"

#ifndef PROGRAM_H
#define PROGRAM_H

class Program {
    Model& model;
    // pushing KeypressMessage to Message queue will slice it down to base class
    // thus, we cannot cast it later. instead, we store _pointers_ to Message.
    // therefore, the actual derived objects (ie; Keypress) stay intact.
    // we use a smart ptr instead of raw - owns a heap-obj, auto del on OOS, cannot copy, only mv
    std::queue<std::unique_ptr<Message>> msgQ;
    bool running = false;
    bool raw = false;

    public:
        Program(Model& m, bool raw) : model(m), raw(raw) {}
        void update(const Message& msg);
        void render();
        void run();
    
    private:
        void handleInput();
        void handleOutput();
};

#endif // PROGRAM_H