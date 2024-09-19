//
// Created by Vlad Boguzh on 18.09.2024.
//

#ifndef DISTANCE_CONSTRAINT_H
#define DISTANCE_CONSTRAINT_H

#include "constraint.h"
#include "../objects/point.h"

class DistanceConstraint final : public Constraint {
public:
    DistanceConstraint(Point& p1, Point& p2, double targetDistance);
    bool isSatisfied() const override;
    void apply() override;

private:
    Point& point1;
    Point& point2;
    double targetDistance;
};

#endif //DISTANCE_CONSTRAINT_H
