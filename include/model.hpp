#ifndef MODEL_H
#define MODEL_H

class Model {
    public:
        // needed for polymorphic deletion (ie; delete parent + derived class from Model)
        virtual ~Model() = default;
};

#endif // MODEL_H