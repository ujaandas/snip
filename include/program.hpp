#include "model.hpp"
#include "message.hpp"

#ifndef PROGRAM_H
#define PROGRAM_H

class Program {
    Model& model;

    public:
        Program(Model& m) : model(m) {}
        void update(Model& m, const Message& msg);
        void render(const Model& m);
};

#endif // PROGRAM_H