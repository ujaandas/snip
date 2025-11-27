#include <iostream>
#include <string>
#include <thread>
#include "program.hpp"
#include "terminal.hpp"
#include "message.hpp"

void Program::handleInput() {
    TerminalHelper term;
    if (raw) term.enableRawMode();

    while (running) {
        char c;
        if (raw) {
            if (read(STDIN_FILENO, &c, 1) > 0)
                msgQ.push(KeypressMessage(c));
        } else {
            int c2 = std::cin.get();
            msgQ.push(KeypressMessage(c2));
        }
    }
}


void Program::handleOutput() {
    while (this->running) {
        if (!this->msgQ.empty()) {
            KeypressMessage msg = msgQ.front();
            msgQ.pop();
            std::cout << "rcv: " << msg.key;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}

void Program::run() {
    this->running = true;
    std::thread input (&Program::handleInput, this);
    std::thread output(&Program::handleOutput, this);

    input.join();
    output.join();
}