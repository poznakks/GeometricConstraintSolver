//
// Created by Vlad Boguzh on 18.09.2024.
//

#include "p2p_distance_constraint.h"

P2PDistanceConstraint::P2PDistanceConstraint(Point& p1, Point& p2, const double targetDistance)
    : point1(p1), point2(p2), targetDistance(targetDistance) {}

bool P2PDistanceConstraint::isSatisfied() const {
    return std::abs(point1.distanceTo(point2) - targetDistance) < 1e-6;
}

void P2PDistanceConstraint::apply() {
    if (!isSatisfied()) {
        const double currentDistance = point1.distanceTo(point2);
        std::cout << "currentDistance " << currentDistance << std::endl;
        const double scale = targetDistance / currentDistance;
        point2.move(
            (point2.x - point1.x) * (scale - 1),
            (point2.y - point1.y) * (scale - 1),
            (point2.z - point1.z) * (scale - 1)
        );
    }
}