//
// Created by Vlad Boguzh on 22.09.2024.
//

#include "circle.h"

#include "plane.h"

Circle::Circle(const Point& center, double radius, const Point& normal)
    : center(center), radius(radius), normal(normal) {}

// Расстояние от точки до окружности
double Circle::distanceTo(const Point& p) const {
    const Plane plane(center, normal);
    const double distanceToPlane = plane.distanceTo(p);
    const auto projection = Point(p.x - distanceToPlane * normal.x,
                                  p.y - distanceToPlane * normal.y,
                                  p.z - distanceToPlane * normal.z);
    const double distanceToCenter = center.distanceTo(projection);
    return std::abs(distanceToCenter - radius);
}

// Расстояние между двумя окружностями
double Circle::distanceTo(const Circle& other) const {
    // Проверяем, находятся ли окружности на одной плоскости
    const Plane thisPlane(center, normal);

    // Если окружности лежат на одной плоскости (или практически на одной)
    if (const double distanceToOtherPlane = thisPlane.distanceTo(other.center); std::abs(distanceToOtherPlane) < 1e-6) {
        // Тогда расстояние между центрами окружностей минус их радиусы
        const double centerDistance = center.distanceTo(other.center);
        return std::max(0.0, centerDistance - (radius + other.radius));
    } else {
        // Если окружности находятся на разных плоскостях
        return distanceToOtherPlane;
    }
}

std::ostream& operator<<(std::ostream& os, const Circle& circle) {
    os << "Circle(Center: " << circle.center << ", Radius: " << circle.radius
       << ", Normal: " << circle.normal << ")";
    return os;
}