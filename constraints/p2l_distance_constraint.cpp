//
// Created by Vlad Boguzh on 14.04.2025.
//

#include "p2l_distance_constraint.h"

P2LDistanceConstraint::P2LDistanceConstraint(Point& p, Line& l, const double targetDistance)
    : point(p), line(l), targetDistance(targetDistance) {}

bool P2LDistanceConstraint::isSatisfied() const {
    return std::abs(line.distanceTo(point) - targetDistance) < 1e-6;
}

void P2LDistanceConstraint::apply() {
    if (!isSatisfied()) {
        const Point& A = line.point;
        const Point& P = point;
        const Point& d = line.direction;

        // Вектор от точки на прямой до точки
        double vx = P.x - A.x;
        double vy = P.y - A.y;

        // Направляющий вектор прямой
        double dx = d.x;
        double dy = d.y;

        // Нормаль к направляющему вектору (перпендикуляр)
        double nx = -dy;
        double ny = dx;

        // Нормализация нормали
        double norm = std::sqrt(nx * nx + ny * ny);
        nx /= norm;
        ny /= norm;

        // Подпись текущего расстояния с учетом направления
        double currentDistance = (vx * nx + vy * ny); // скалярное проецирование на нормаль
        std::cout << "currentDistance " << currentDistance << std::endl;

        double delta = targetDistance - currentDistance;

        // Сдвиг точки
        point.move(nx * delta, ny * delta, 0);
    }
}