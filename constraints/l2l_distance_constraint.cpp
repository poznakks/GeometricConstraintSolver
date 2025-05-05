//
// Created by Vlad Boguzh on 20.04.2025.
//

#include "l2l_distance_constraint.h"

L2LDistanceConstraint::L2LDistanceConstraint(LineSharedPtr line1, LineSharedPtr line2, const double distance)
    : line1(std::move(line1)), line2(std::move(line2)), distance(distance) {}

void L2LDistanceConstraint::apply() {
    Point dir1 = line1->direction.normalized();
    line2->direction = dir1;

    Point offset = line2->point - line1->point;
    double projectionLength = offset.dot(dir1);
    Point projection = dir1 * projectionLength;
    Point perpendicular = offset - projection;

    Point correction = perpendicular.normalized() * distance;
    line2->point = line1->point + correction;
}

bool L2LDistanceConstraint::isSatisfied() const {
    // Проверим, что направления совпадают (или противоположны, если допускается)
    Point d1 = line1->direction.normalized();
    Point d2 = line2->direction.normalized();
    double dotProduct = d1.dot(d2);
    bool parallel = std::abs(std::abs(dotProduct) - 1.0) < 1e-5;

    // Проверим расстояние между линиями
    Point offset = line2->point - line1->point;
    double projectionLength = offset.dot(d1);
    Point projection = d1 * projectionLength;
    Point perpendicular = offset - projection;

    double actualDistance = perpendicular.length();

    return parallel && std::abs(actualDistance - distance) < 1e-3;
}

ObjectSharedPtr L2LDistanceConstraint::getObjectA() {
    return line1;
}

ObjectSharedPtr L2LDistanceConstraint::getObjectB() {
    return line2;
}