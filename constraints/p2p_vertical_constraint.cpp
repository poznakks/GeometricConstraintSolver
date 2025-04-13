//
// Created by Vlad Boguzh on 18.09.2024.
//

#include "p2p_vertical_constraint.h"

P2PVerticalConstraint::P2PVerticalConstraint(Point& p1, Point& p2)
    : point1(p1), point2(p2) {}

bool P2PVerticalConstraint::isSatisfied() const {
    return point1.x == point2.x && point1.z == point2.z;
}

void P2PVerticalConstraint::apply() {
    if (!isSatisfied()) {
        point2.x = point1.x;
        point2.z = point1.z;
    }
}