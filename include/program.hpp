#include <queue>
#include "model.hpp"
#include "message.hpp"

#ifndef PROGRAM_H
#define PROGRAM_H

class Program {
    Model& model;
    std::queue<KeypressMessage> msgQ;
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