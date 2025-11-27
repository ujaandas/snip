#include <iostream>
#include <string>
#include <thread>
#include "program.hpp"
#include "terminal.hpp"
#include "message.hpp"

void Program::handleInput() {
    TerminalHelper term;
    if (this->raw) term.enableRawMode();
    term.disableEcho();

    while (this->running) {
        char c;
        if (this->raw) {
            if (read(STDIN_FILENO, &c, 1) > 0)
                msgQ.push(std::make_unique<KeypressMessage>(c));
        } else {
            int c2 = std::cin.get();
            msgQ.push(std::make_unique<KeypressMessage>(c2));
        }
    }
}

void Program::handleOutput() {
    while (this->running) {
        if (!msgQ.empty()) {
            auto msg = std::move(msgQ.front());
            msgQ.pop();
            this->update(*msg);
            this->render();
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}

void Program::run() {
    this->running = true;
    std::thread input (&Program::handleInput, this);
    std::thread output (&Program::handleOutput, this);

    input.join();
    output.join();
}