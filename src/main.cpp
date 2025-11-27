#include <iostream>
#include "program.hpp"

class CounterModel : public Model {
    public:
        int count = 0;
};

enum class CounterMessageType { Increment, Quit };

class CounterMessage : public Message {
public:
    CounterMessageType type;
    CounterMessage(CounterMessageType t) : type(t) {}
};


void Program::update(Model& m, const Message& msg) {
    CounterModel& cm = static_cast<CounterModel&>(m);
    const CounterMessage& cmsg = static_cast<const CounterMessage&>(msg);

   switch (cmsg.type) {
        case CounterMessageType::Increment:
            cm.count++;
            break;
        case CounterMessageType::Quit:
            std::cout << "Quitting...\n";
            break;
    }
}

void Program::render(const Model& m) {
    const auto& model = dynamic_cast<const CounterModel&>(m);
    std::cout << "Count: " << model.count << std::endl;
}

int main() {
    CounterModel model;
    Program program(model);

    program.update(model, CounterMessage(CounterMessageType::Increment));
    program.render(model);

    program.update(model, CounterMessage(CounterMessageType::Increment));
    program.render(model);

    program.update(model, CounterMessage(CounterMessageType::Quit));

    return 0;
}