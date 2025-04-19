//
// Created by Vlad Boguzh on 18.09.2024.
//

#ifndef DISTANCE_CONSTRAINT_H
#define DISTANCE_CONSTRAINT_H

#include "constraint.h"
#include "../types.h"
#include "../objects/point.h"

class P2PDistanceConstraint final : public Constraint {
public:
    P2PDistanceConstraint(PointSharedPtr p1, PointSharedPtr p2, double targetDistance);
    bool isSatisfied() const override;
    void apply() override;

private:
    PointSharedPtr point1;
    PointSharedPtr point2;
    double targetDistance;
};

#endif //DISTANCE_CONSTRAINT_H
