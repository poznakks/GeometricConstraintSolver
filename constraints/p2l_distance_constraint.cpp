//
// Created by Vlad Boguzh on 14.04.2025.
//

#include "p2l_distance_constraint.h"

P2LDistanceConstraint::P2LDistanceConstraint(PointSharedPtr p, LineSharedPtr l, const double targetDistance)
    : point(std::move(p)), line(std::move(l)), targetDistance(targetDistance) {}

bool P2LDistanceConstraint::isSatisfied() const {
    return std::abs(line->distanceTo(*point) - targetDistance) < 1e-6;
}

void P2LDistanceConstraint::apply() {
    if (!isSatisfied()) {
        const Point& A = line->point;     // Точка на прямой
        const Point& d = line->direction; // Направляющий вектор прямой
        const Point& P = *point;          // Точка, которую двигаем

        // Вектор от A до P
        double vx = P.x - A.x;
        double vy = P.y - A.y;
        double vz = P.z - A.z;

        // Проекция вектора AP на направление прямой
        double dot = vx * d.x + vy * d.y + vz * d.z;
        double lenSq = d.x * d.x + d.y * d.y + d.z * d.z;
        double t = dot / lenSq;

        // Координаты проекции точки на прямую
        double projX = A.x + d.x * t;
        double projY = A.y + d.y * t;
        double projZ = A.z + d.z * t;

        // Вектор от проекции до исходной точки P (нормаль к прямой)
        double nx = P.x - projX;
        double ny = P.y - projY;
        double nz = P.z - projZ;

        double norm = std::sqrt(nx * nx + ny * ny + nz * nz);
        if (norm < 1e-8) return; // точка уже лежит на прямой — нет нормали

        nx /= norm;
        ny /= norm;
        nz /= norm;

        double delta = targetDistance - norm;

        point->move(nx * delta, ny * delta, nz * delta);
    }
}