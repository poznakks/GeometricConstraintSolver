//
// Created by Vlad Boguzh on 18.09.2024.
//

#include "p2p_horizontal_constraint.h"

P2PHorizontalConstraint::P2PHorizontalConstraint(Point& p1, Point& p2)
    : point1(p1), point2(p2) {}

bool P2PHorizontalConstraint::isSatisfied() const {
    return point1.y == point2.y && point1.z == point2.z;
}

void P2PHorizontalConstraint::apply() {
    if (!isSatisfied()) {
        point2.y = point1.y;
        point2.z = point1.z;
    }
}