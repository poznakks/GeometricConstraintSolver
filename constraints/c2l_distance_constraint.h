//
// Created by Vlad Boguzh on 21.05.2025.
//

#ifndef C2L_DISTANCE_CONSTRAINT_H
#define C2L_DISTANCE_CONSTRAINT_H
#include "constraint.h"
#include "../types.h"

class C2LDistanceConstraint final : public Constraint {
public:
    C2LDistanceConstraint(CircleSharedPtr c, LineSharedPtr l, double targetDistance);
    bool isSatisfied() const override;
    void apply() override;
    ObjectSharedPtr getObjectA() override;
    ObjectSharedPtr getObjectB() override;

private:
    CircleSharedPtr circle;
    LineSharedPtr line;
    double targetDistance;
};

#endif //C2L_DISTANCE_CONSTRAINT_H
