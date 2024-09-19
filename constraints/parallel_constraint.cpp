//
// Created by Vlad Boguzh on 18.09.2024.
//

#include "parallel_constraint.h"

ParallelConstraint::ParallelConstraint(Line& line1, Line& line2)
    : line1(line1), line2(line2) {}

bool ParallelConstraint::isSatisfied() const {
    // Векторное произведение должно быть равно нулю, если линии параллельны
    const Point crossProduct(
        line1.direction.y * line2.direction.z - line1.direction.z * line2.direction.y,
        line1.direction.z * line2.direction.x - line1.direction.x * line2.direction.z,
        line1.direction.x * line2.direction.y - line1.direction.y * line2.direction.x
    );
    return (crossProduct.x == 0 && crossProduct.y == 0 && crossProduct.z == 0);
}

void ParallelConstraint::apply() {
    if (!isSatisfied()) {
        // Применяем направляющий вектор первой прямой ко второй
        line2.direction = line1.direction;
    }
}