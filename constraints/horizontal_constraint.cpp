//
// Created by Vlad Boguzh on 18.09.2024.
//

#include "horizontal_constraint.h"

HorizontalConstraint::HorizontalConstraint(Point& p1, Point& p2)
    : point1(p1), point2(p2) {}

bool HorizontalConstraint::isSatisfied() const {
    return point1.y == point2.y && point1.z == point2.z;
}

void HorizontalConstraint::apply() {
    if (!isSatisfied()) {
        point2.y = point1.y;
        point2.z = point1.z;
    }
}