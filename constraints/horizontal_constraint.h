//
// Created by Vlad Boguzh on 18.09.2024.
//

#ifndef HORIZONTAL_CONSTRAINT_H
#define HORIZONTAL_CONSTRAINT_H

#include "constraint.h"
#include "../objects/point.h"

class HorizontalConstraint final : public Constraint {
public:
    HorizontalConstraint(Point& p1, Point& p2);
    bool isSatisfied() const override;
    void apply() override;

private:
    Point& point1;
    Point& point2;
};

#endif //HORIZONTAL_CONSTRAINT_H
