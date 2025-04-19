//
// Created by Vlad Boguzh on 18.09.2024.
//

#include "parallel_constraint.h"

ParallelConstraint::ParallelConstraint(LineSharedPtr l1, LineSharedPtr l2)
    : line1(std::move(l1)), line2(std::move(l2)) {}

bool ParallelConstraint::isSatisfied() const {
    const Point& d1 = line1->direction;
    const Point& d2 = line2->direction;

    // Векторное произведение должно быть равно нулю, если линии параллельны
    const Point crossProduct(
        d1.y * d2.z - d1.z * d2.y,
        d1.z * d2.x - d1.x * d2.z,
        d1.x * d2.y - d1.y * d2.x
    );
    return (crossProduct.x == 0 && crossProduct.y == 0 && crossProduct.z == 0);
}

void ParallelConstraint::apply() {
    if (!isSatisfied()) {
        line2->direction = line1->direction;
    }
}