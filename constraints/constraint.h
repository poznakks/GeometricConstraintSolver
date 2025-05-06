//
// Created by Vlad Boguzh on 18.09.2024.
//

#ifndef CONSTRAINT_H
#define CONSTRAINT_H
#include <memory>

#include "../objects/geometric_object.h"

class Constraint {
public:
    virtual ~Constraint() = default;
    virtual bool isSatisfied() const = 0;
    virtual void apply() = 0;
    virtual std::shared_ptr<GeometricObject> getObjectA() = 0;
    virtual std::shared_ptr<GeometricObject> getObjectB() = 0;
};

#endif //CONSTRAINT_H
