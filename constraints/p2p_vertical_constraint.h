//
// Created by Vlad Boguzh on 18.09.2024.
//

#ifndef VERTICAL_CONSTRAINT_H
#define VERTICAL_CONSTRAINT_H

#include "constraint.h"
#include "../types.h"
#include "../objects/point.h"

class P2PVerticalConstraint final : public Constraint {
public:
    P2PVerticalConstraint(PointSharedPtr p1, PointSharedPtr p2);
    bool isSatisfied() const override;
    void apply() override;
    ObjectSharedPtr getObjectA() override;
    ObjectSharedPtr getObjectB() override;

private:
    PointSharedPtr point1;
    PointSharedPtr point2;
};

#endif //VERTICAL_CONSTRAINT_H
