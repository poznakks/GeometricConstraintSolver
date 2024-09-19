//
// Created by Vlad Boguzh on 18.09.2024.
//

#ifndef PARALLEL_CONSTRAINT_H
#define PARALLEL_CONSTRAINT_H

#include "constraint.h"
#include "../objects//line.h"

class ParallelConstraint final : public Constraint {
public:
    ParallelConstraint(Line& line1, Line& line2);
    bool isSatisfied() const override;
    void apply() override;

private:
    Line& line1;
    Line& line2;
};

#endif //PARALLEL_CONSTRAINT_H
