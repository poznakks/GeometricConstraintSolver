//
// Created by Vlad Boguzh on 22.09.2024.
//

#include "circle.h"

#include "plane.h"

Circle::Circle(const Point& center, double radius, const Point& normal)
    : center(center), radius(radius), normal(normal.normalized()) {}

bool Circle::isPointOnCircle(const Point& point) const {
    Point vec = point - center;
    double distToPlane = std::abs(vec.dot(normal));
    if (distToPlane > 1e-6) return false;

    double radialDist = vec.length();
    return std::abs(radialDist - radius) < 1e-6;
}

bool Circle::isLineTangent(const Line& line) const {
    // Прямая должна лежать в плоскости круга
    double directionDot = std::abs(line.direction.dot(normal));
    if (directionDot > 1e-6) return false; // не в плоскости

    double pointDot = std::abs((line.point - center).dot(normal));
    if (pointDot > 1e-6) return false; // не в плоскости

    // Найдём кратчайшее расстояние от центра до прямой
    Point vecToLine = line.point - center;
    Point proj = line.direction.normalized();
    Point closest = line.point - proj * (vecToLine.dot(proj));

    double dist = (closest - center).length();
    return std::abs(dist - radius) < 1e-6;
}