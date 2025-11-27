#include <iostream>
#include "program.hpp"

struct CounterModel : Model {
    int count = 0;
};

struct IncrementMessage : Message {
    int amount;
    IncrementMessage(int amt) : amount(amt) {}
};

struct QuitMessage : Message {
    std::string reason;
    QuitMessage(std::string r = "User requested quit") : reason(std::move(r)) {}
};

void Program::update(const Message& msg) {
    CounterModel& m = static_cast<CounterModel&>(this->model);

    if (auto inc = dynamic_cast<const IncrementMessage*>(&msg)) {
        m.count += inc->amount;
    }
    else if (auto quit = dynamic_cast<const QuitMessage*>(&msg)) {
        std::cout << "Quitting... Reason: " << quit->reason << "\n";
    }
}

void Program::render() {
    const CounterModel& m = static_cast<const CounterModel&>(this->model);
    std::cout << "Count: " << m.count << std::endl;
}

int main() {
    CounterModel model;
    Program program(model, true);

    program.run();

    // program.update(IncrementMessage(1));
    // program.render();

    // program.update(IncrementMessage(3));
    // program.render();

    // program.update(QuitMessage("done"));

    return 0;
}