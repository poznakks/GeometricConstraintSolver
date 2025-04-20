//
// Created by Vlad Boguzh on 20.04.2025.
//

#ifndef L2L_PERPENDICULAR_CONSTRAINT_H
#define L2L_PERPENDICULAR_CONSTRAINT_H
#include "constraint.h"
#include "../types.h"

class L2LPerpendicularConstraint final : public Constraint {
public:
    L2LPerpendicularConstraint(LineSharedPtr l1, LineSharedPtr l2);
    bool isSatisfied() const override;
    void apply() override;

private:
    LineSharedPtr line1;
    LineSharedPtr line2;
};

#endif //L2L_PERPENDICULAR_CONSTRAINT_H
