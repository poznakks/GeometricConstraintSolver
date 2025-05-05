//
// Created by Vlad Boguzh on 14.04.2025.
//

#ifndef P2L_DISTANCE_CONSTRAINT_H
#define P2L_DISTANCE_CONSTRAINT_H

#include "constraint.h"
#include "../types.h"

class P2LDistanceConstraint final : public Constraint {
public:
    P2LDistanceConstraint(PointSharedPtr p, LineSharedPtr l, double targetDistance);
    bool isSatisfied() const override;
    void apply() override;
    ObjectSharedPtr getObjectA() override;
    ObjectSharedPtr getObjectB() override;

private:
    PointSharedPtr point;
    LineSharedPtr line;
    double targetDistance;
};

#endif //P2L_DISTANCE_CONSTRAINT_H
