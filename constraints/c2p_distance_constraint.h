//
// Created by Vlad Boguzh on 21.05.2025.
//

#ifndef C2P_DISTANCE_CONSTRAINT_H
#define C2P_DISTANCE_CONSTRAINT_H

#include "constraint.h"
#include "../types.h"

class C2PDistanceConstraint final : public Constraint {
public:
    C2PDistanceConstraint(CircleSharedPtr c, PointSharedPtr p, double targetDistance);
    bool isSatisfied() const override;
    void apply() override;
    ObjectSharedPtr getObjectA() override;
    ObjectSharedPtr getObjectB() override;

private:
    CircleSharedPtr circle;
    PointSharedPtr point;
    double targetDistance;
};

#endif //C2P_DISTANCE_CONSTRAINT_H
