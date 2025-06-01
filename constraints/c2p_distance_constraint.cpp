//
// Created by Vlad Boguzh on 21.05.2025.
//

#include "c2p_distance_constraint.h"

C2PDistanceConstraint::C2PDistanceConstraint(CircleSharedPtr c, PointSharedPtr p, const double targetDistance)
    : circle(std::move(c)), point(std::move(p)), targetDistance(targetDistance) {}

bool C2PDistanceConstraint::isSatisfied() const {
    return std::abs(circle->center.distanceTo(*point) - targetDistance) < 1e-6;
}

void C2PDistanceConstraint::apply() {
    if (!isSatisfied()) {
        const double currentDistance = circle->center.distanceTo(*point);
        const double scale = targetDistance / currentDistance;
        point->move(
            (point->x - circle->center.x) * (scale - 1),
            (point->y - circle->center.y) * (scale - 1),
            (point->z - circle->center.z) * (scale - 1)
        );
    }
}

ObjectSharedPtr C2PDistanceConstraint::getObjectA() {
    return circle;
}

ObjectSharedPtr C2PDistanceConstraint::getObjectB() {
    return point;
}