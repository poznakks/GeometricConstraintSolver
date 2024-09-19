//
// Created by Vlad Boguzh on 18.09.2024.
//

#include "vertical_constraint.h"

VerticalConstraint::VerticalConstraint(Point& p1, Point& p2)
    : point1(p1), point2(p2) {}

bool VerticalConstraint::isSatisfied() const {
    return point1.x == point2.x && point1.z == point2.z;
}

void VerticalConstraint::apply() {
    if (!isSatisfied()) {
        point2.x = point1.x;
        point2.z = point1.z;
    }
}