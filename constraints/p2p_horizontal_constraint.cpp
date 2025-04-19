//
// Created by Vlad Boguzh on 18.09.2024.
//

#include "p2p_horizontal_constraint.h"

P2PHorizontalConstraint::P2PHorizontalConstraint(PointSharedPtr p1, PointSharedPtr p2)
    : point1(std::move(p1)), point2(std::move(p2)) {}

bool P2PHorizontalConstraint::isSatisfied() const {
    return point1->y == point2->y && point1->z == point2->z;
}

void P2PHorizontalConstraint::apply() {
    if (!isSatisfied()) {
        point2->y = point1->y;
        point2->z = point1->z;
    }
}