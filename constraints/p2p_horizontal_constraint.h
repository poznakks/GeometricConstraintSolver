//
// Created by Vlad Boguzh on 18.09.2024.
//

#ifndef HORIZONTAL_CONSTRAINT_H
#define HORIZONTAL_CONSTRAINT_H

#include "constraint.h"
#include "../types.h"
#include "../objects/point.h"

class P2PHorizontalConstraint final : public Constraint {
public:
    P2PHorizontalConstraint(PointSharedPtr p1, PointSharedPtr p2);
    bool isSatisfied() const override;
    void apply() override;

private:
    PointSharedPtr point1;
    PointSharedPtr point2;
};
#endif //HORIZONTAL_CONSTRAINT_H
