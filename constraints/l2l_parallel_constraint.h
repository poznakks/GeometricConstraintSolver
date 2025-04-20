//
// Created by Vlad Boguzh on 18.09.2024.
//

#ifndef PARALLEL_CONSTRAINT_H
#define PARALLEL_CONSTRAINT_H

#include "constraint.h"
#include "../types.h"
#include "../objects//line.h"

class L2LParallelConstraint final : public Constraint {
public:
    L2LParallelConstraint(LineSharedPtr l1, LineSharedPtr l2);
    bool isSatisfied() const override;
    void apply() override;

private:
    LineSharedPtr line1;
    LineSharedPtr line2;
};


#endif //PARALLEL_CONSTRAINT_H
