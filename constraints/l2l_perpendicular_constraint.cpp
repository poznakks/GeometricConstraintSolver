//
// Created by Vlad Boguzh on 20.04.2025.
//

#include "l2l_perpendicular_constraint.h"

L2LPerpendicularConstraint::L2LPerpendicularConstraint(LineSharedPtr l1, LineSharedPtr l2)
    : line1(std::move(l1)), line2(std::move(l2)) {}

bool L2LPerpendicularConstraint::isSatisfied() const {
    Point dir1 = line1->direction.normalized();
    Point dir2 = line2->direction.normalized();
    double dot = dir1.dot(dir2);
    return std::abs(dot) < 1e-4;
}

void L2LPerpendicularConstraint::apply() {
    if (isSatisfied()) return;

    Point dir1 = line1->direction.normalized();

    // Вектор, перпендикулярный dir1
    Point newDir(-dir1.y, dir1.x);
    line2->direction = newDir.normalized();
    line2->setDirty(true);
}

ObjectSharedPtr L2LPerpendicularConstraint::getObjectA() {
    return line1;
}

ObjectSharedPtr L2LPerpendicularConstraint::getObjectB() {
    return line2;
}