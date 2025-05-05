//
// Created by Vlad Boguzh on 18.09.2024.
//

#include "p2p_distance_constraint.h"
#include "../types.h"

P2PDistanceConstraint::P2PDistanceConstraint(PointSharedPtr p1, PointSharedPtr p2, const double targetDistance)
    : point1(std::move(p1)), point2(std::move(p2)), targetDistance(targetDistance) {}

bool P2PDistanceConstraint::isSatisfied() const {
    return std::abs(point1->distanceTo(*point2) - targetDistance) < 1e-6;
}

void P2PDistanceConstraint::apply() {
    if (!isSatisfied()) {
        const double currentDistance = point1->distanceTo(*point2);
        const double scale = targetDistance / currentDistance;
        point2->move(
            (point2->x - point1->x) * (scale - 1),
            (point2->y - point1->y) * (scale - 1),
            (point2->z - point1->z) * (scale - 1)
        );
    }
}

ObjectSharedPtr P2PDistanceConstraint::getObjectA() {
    return point1;
}

ObjectSharedPtr P2PDistanceConstraint::getObjectB() {
    return point2;
}