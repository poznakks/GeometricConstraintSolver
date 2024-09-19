//
// Created by Vlad Boguzh on 18.09.2024.
//

#ifndef VERTICAL_CONSTRAINT_H
#define VERTICAL_CONSTRAINT_H

#include "constraint.h"
#include "../objects/point.h"

class VerticalConstraint final : public Constraint {
public:
    VerticalConstraint(Point& p1, Point& p2);
    bool isSatisfied() const override;
    void apply() override;

private:
    Point& point1;
    Point& point2;
};

#endif //VERTICAL_CONSTRAINT_H
