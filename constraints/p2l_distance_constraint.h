//
// Created by Vlad Boguzh on 14.04.2025.
//

#ifndef P2L_DISTANCE_CONSTRAINT_H
#define P2L_DISTANCE_CONSTRAINT_H

#include "constraint.h"
#include "../objects/line.h"

class P2LDistanceConstraint final : public Constraint {
public:
    P2LDistanceConstraint(Point& p, Line& l, double targetDistance);
    bool isSatisfied() const override;
    void apply() override;

private:
    Point& point;
    Line& line;
    double targetDistance;
};

#endif //P2L_DISTANCE_CONSTRAINT_H
