//
// Created by Vlad Boguzh on 22.09.2024.
//

#include "ellipse.h"

#include "plane.h"

Ellipse::Ellipse(const Point& center, double semiAxisA, double semiAxisB, const Point& normal)
    : center(center), semiAxisA(semiAxisA), semiAxisB(semiAxisB), normal(normal) {}

// Расстояние от точки до эллипса (приближенно, если точка не находится на плоскости эллипса)
double Ellipse::distanceTo(const Point& p) const {
    const Plane plane(center, normal);
    const double distanceToPlane = plane.distanceTo(p);
    const auto projection = Point(p.x - distanceToPlane * normal.x,
                                  p.y - distanceToPlane * normal.y,
                                  p.z - distanceToPlane * normal.z);
    const double dx = projection.x - center.x;
    const double dy = projection.y - center.y;
    const double dist = std::sqrt((dx * dx) / (semiAxisA * semiAxisA) + (dy * dy) / (semiAxisB * semiAxisB));
    return std::abs(dist - 1.0);
}

std::ostream& operator<<(std::ostream& os, const Ellipse& ellipse) {
    os << "Ellipse(Center: " << ellipse.center << ", Semi-Axis A: " << ellipse.semiAxisA
       << ", Semi-Axis B: " << ellipse.semiAxisB << ", Normal: " << ellipse.normal << ")";
    return os;
}