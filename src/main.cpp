#include <iostream>
#include "program.hpp"

struct CounterModel : Model {
    int count = 0;
};

struct IncrementMessage : public Message {
    IncrementMessage(int amt) : amount(amt) {}
    int amount{};
};

struct QuitMessage : public Message {
    QuitMessage(std::string r = "User requested quit") : reason(std::move(r)) {}
    std::string reason;
};

void Program::update(Model& m, const Message& msg) {
    CounterModel& cm = static_cast<CounterModel&>(m);

    if (auto inc = dynamic_cast<const IncrementMessage*>(&msg)) {
        cm.count += inc->amount;
    }
    else if (auto quit = dynamic_cast<const QuitMessage*>(&msg)) {
        std::cout << "Quitting... Reason: " << quit->reason << "\n";
    }
}

void Program::render(const Model& m) {
    const CounterModel& model = static_cast<const CounterModel&>(m);
    std::cout << "Count: " << model.count << std::endl;
}

int main() {
    CounterModel model;
    Program program(model);

    program.update(model, IncrementMessage(1));
    program.render(model);

    program.update(model, IncrementMessage(3));
    program.render(model);

    program.update(model, QuitMessage("done"));

    return 0;
}