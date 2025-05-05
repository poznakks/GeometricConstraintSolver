//
// Created by Vlad Boguzh on 20.04.2025.
//

#ifndef L2L_DISTANCE_CONSTRAINT_H
#define L2L_DISTANCE_CONSTRAINT_H

#include "constraint.h"
#include "../types.h"

class L2LDistanceConstraint : public Constraint {
public:
    L2LDistanceConstraint(LineSharedPtr line1, LineSharedPtr line2, double distance);
    void apply() override;
    bool isSatisfied() const override;
    ObjectSharedPtr getObjectA() override;
    ObjectSharedPtr getObjectB() override;

private:
    LineSharedPtr line1;
    LineSharedPtr line2;
    double distance;
};

#endif //L2L_DISTANCE_CONSTRAINT_H
