//
// Created by Vlad Boguzh on 18.09.2024.
//

#ifndef CONSTRAINT_H
#define CONSTRAINT_H

class Constraint {
public:
    virtual ~Constraint() = default;
    virtual bool isSatisfied() const = 0;
    virtual void apply() = 0;
};

#endif //CONSTRAINT_H
